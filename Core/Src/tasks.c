#include "tasks.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "display.h"
#include "ee24.h"
#include "i2c.h"
#include "main.h"
#include "navigation.h"
#include "screens.h"
#include "settings.h"
#include "setupscreen.h"
#include "task.h"
#include "utils.h"
#include "w25qxx.h"
#include <arm_const_structs.h>
#include <arm_math.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) a >= b ? a : b

extern ADC_HandleTypeDef MEASURE_ADC_Handle;
extern TIM_HandleTypeDef FREQUENCY_TIM_Handle;
extern TIM_HandleTypeDef LED_TIM_Handle;
extern TIM_HandleTypeDef FREQUENCY_TIM_Handle;
extern RTC_HandleTypeDef RTC_Handle;
extern SPI_HandleTypeDef FLASH_SPI_Handle;

extern osThreadId_t adcTaskHandle;
extern osThreadId_t relSCTaskHandle;
extern osThreadId_t fftTaskHandle;
extern osThreadId_t relTaskHandle;
extern osThreadId_t btnsTaskHandle;
extern osThreadId_t displTaskHandle;
extern osThreadId_t rtcTaskHandle;
extern osThreadId_t flashTaskHandle;

void SetAdcCaptureTimerEvent(void) { osThreadFlagsSet(adcTaskHandle, START_ADC); }

void StartAdcCaptureTask(void *argument) {
  HAL_ADC_Start_DMA(&MEASURE_ADC_Handle, (uint32_t *)&general.sensors.adc_value, 5);
  HAL_TIM_Base_Start_IT(&FREQUENCY_TIM_Handle);
  __HAL_TIM_ENABLE_IT(&FREQUENCY_TIM_Handle, TIM_IT_UPDATE);
  HAL_TIM_IC_Start_IT(&FREQUENCY_TIM_Handle, TIM_CHANNEL_3);
  uint32_t flags = 0;

  for (;;) {
    flags = osThreadFlagsWait(START_ADC, osFlagsWaitAny, osWaitForever);
    if (general.state.recalcAdcZero == 1) {
      uint32_t avgCurZero = 0, avgVolZero = 0;
      for (int i = 0; i < FFT_SIZE; i++) {
        // avgVolZero += general.sensors.voltage_values[i];
        avgCurZero += general.sensors.current_values[i];
      }
      // general.settings.voltageZeroAdc = avgVolZero / FFT_SIZE;
      general.settings.currentZeroAdc = avgCurZero / FFT_SIZE;
      general.state.recalcAdcZero = 0;
    } else {
      general.sensors.voltage =
          RECOUNT_VOLTAGE(CountRmsValueX100(general.sensors.voltage_values, FFT_SIZE, general.settings.voltageZeroAdc)); //Коэф пересчета в В
      general.sensors.current =
          RECOUNT_CURRENT(CountRmsValueX100(general.sensors.current_values, FFT_SIZE, general.settings.currentZeroAdc)); //Коэф пересчета в А
    }
    osThreadFlagsSet(fftTaskHandle, START_FFT);
    osThreadFlagsSet(relSCTaskHandle, VOLTAGE_CURRENT_RMS);
  }
}

void StartFftTask(void *argument) {
  uint32_t flags = 0;
  arm_rfft_fast_instance_f32 rfft_inst;
  arm_rfft_fast_init_f32(&rfft_inst, FFT_SIZE);
  
  for (;;) {
    flags = osThreadFlagsWait(START_FFT, osFlagsWaitAny, osWaitForever);

    arm_rfft_fast_f32(&rfft_inst, general.sensors.fft_voltage_in + (FFT_SIZE * general.sensors.values_bufNum), general.sensors.fft_voltage_out, 0);
    arm_cmplx_mag_f32(general.sensors.fft_voltage_out, general.sensors.fft_voltage_out, FFT_SIZE);

    general.sensors.sin = general.sensors.fft_voltage_out[PERIODS_COUNT];
    general.sensors.harm = 0;
    for (int i = 2 * PERIODS_COUNT; i < FFT_SIZE / 2; i = i + PERIODS_COUNT) {
      general.sensors.harm += general.sensors.fft_voltage_out[i] * general.sensors.fft_voltage_out[i];
    }
    osThreadFlagsSet(relSCTaskHandle, VOLTAGE_FFT);
  }
}

void SetUpdateTickEvent(void) { osThreadFlagsSet(relSCTaskHandle, SYS_TICK); }

void StartFlashTask(void *argument) {
  uint32_t flags = 0;
  uint16_t tempErrorValue = 0;
  flags = osThreadFlagsWait(SPI_DONE, osFlagsWaitAny, osWaitForever);
  MX_I2C1_Init();
  if (ee24_isConnected()) {
    loadEeErrors();
    if (general.eeErrors.errorsCount == 0xFF) {
      memset(&general.eeErrors, 0, sizeof(ee_errors_t));
      saveEeErrors();
    }
    osThreadFlagsWait(RTC_DONE, osFlagsWaitAny, osWaitForever);
    tempErrorValue = general.eeErrors.errors[PowerOffError - 1].value;
    loadErrors();
    loadCrashes();
    // Если ошибка по отключению питания была записана впервые, то её нужно сохранить как уже обработанную
    if (general.screens.errorsScrValues.errors[PowerOffError].value != tempErrorValue)
      saveEeErrors();

    loadEeSettings();
    if (general.settings.maxVoltage == 0xFFFF) {
      resetSettings();
      saveEeSettings();
    }

    loadEeTime();
    if (general.eeTime.daysWork == 0xFFFF) {
      memset(&general.eeTime, 0, sizeof(ee_time_t));
      saveEeTime();
    }
  }
  for (;;) {
    flags = osThreadFlagsWait(0x7FFFFFFF, osFlagsWaitAny, osWaitForever);
    if (flags & SETTINGS_CHANGED)
      saveEeSettings();
    if (flags & ERRORS_CHANGED)
      saveEeErrors();
    if (flags & TIME_CHANGED)
      saveEeTime();
  }
}

static void EnableRelay(void) { osThreadFlagsSet(relTaskHandle, RELAY_ON); }
static void DisableRelay(void) { osThreadFlagsSet(relTaskHandle, RELAY_OFF); }

// 1: есть ошибки, 2: параметры нормализовались (запись), 4: краш, без автоматического включения, 8: первое появление краша (запись)
typedef enum { HasErrors = 1, HasErrorsToSave = 2, HasCrashes = 4, HasCrashesToSave = 8 } HasErrorFlags;

static uint32_t errorsFlag = 0, crashesFlag = 0, errorsToSaveFlag = 0;
static uint32_t timeFromError[ERR_MAX_COUNT], timeFromCrash[CRASH_MAX_COUNT], tickFromError[ERR_MAX_COUNT], tickFromCrash[CRASH_MAX_COUNT];
static err_record_t tempPowerOffError;

static uint8_t greaterPred(uint16_t op1, uint16_t op2) { return op1 >= op2; }
static uint8_t lessPred(uint16_t op1, uint16_t op2) { return op1 < op2; }
static uint8_t truePred(uint16_t op1, uint16_t op2) { return 1; }
static uint8_t (*getGreaterLessPred(uint16_t value, uint16_t less, uint16_t greater))(uint16_t op1, uint16_t op2) {
  if (value >= greater)
    return greaterPred;
  else
    return lessPred;
}
static void processErrorDelay(ErrorType err) {
  if (err == VoltageAmpError) {
    general.state.enableDelayMs = MAX(general.state.enableDelayMs, 60 * 1000 * 180);
    return;
  } else if (err == ManualPowerOffError) {
    // general.state.enableDelayMs = 0;
    return;
  }
  if (timeFromError[err - 1] >= 20 * 60 * 1000)
    general.state.errorDelays[err - 1] = 0;
  switch (general.state.errorDelays[err - 1]) {
  case 0:
    general.state.enableDelayMs = MAX(general.state.enableDelayMs, general.settings.startDelayS * 1000);
    general.state.errorDelays[err - 1] = 1;
    return;
    break;
  case 2:
    general.state.errorDelays[err - 1] = 6;
    break;
  case 24:
    break;
  default:
    general.state.errorDelays[err - 1] *= 2;
    break;
  }
  general.state.enableDelayMs = MAX(general.state.enableDelayMs, 60 * 60 * 1000 * general.state.errorDelays[err - 1]);
}

static uint8_t checkError(uint8_t condition, ErrorType err, uint16_t value, uint32_t timeToSave, uint8_t (*updatePred)(uint16_t, uint16_t),
                          uint8_t saveCondition) {
  HasErrorFlags hasError = 0;
  if (condition) {
    hasError = HasErrors;
    if (general.screens.mainScrValues.errors[err] == 0) { // Ошибка произошла только что
      timeFromError[err - 1] = 0;
      tickFromError[err - 1] = HAL_GetTick();
      setError(err, value);
    } else if (timeFromError[err - 1] < timeToSave)
      ; // setError(err, value);
    else if (saveCondition) {
      if ((errorsFlag & (1 << err)) != 0 || (errorsToSaveFlag & (1 << err)) != 0) { // Старт ошибки уже записан и требуется только обновление значений
        if (updatePred(value, general.screens.errorsScrValues.errors[err].value)) {
          general.eeErrors.errors[err - 1].value = value;
          general.screens.errorsScrValues.errors[err].value = value;
          if (err == VoltageAmpError)
            setError(err, value);
        }
      } else { // Запись ошибки во время её начала
        DisableRelay();
        errorsFlag |= 1 << err;
        saveError(err, value, &general.settings.currentDate, &general.settings.currentDate);
      }
    }
  } else if ((errorsFlag & (1 << err)) != 0) { // Параметры нормализовались
    hasError = HasErrorsToSave;
    errorsFlag &= ~(1 << err);
    errorsToSaveFlag |= 1 << err;
    // setDateTo(err, &general.settings.currentDate);
    resetError(err);
    processErrorDelay(err);
  } else if (general.screens.mainScrValues.errors[err] != 0) {
    resetError(err);
  }
  timeFromError[err - 1] += HAL_GetTick() - tickFromError[err - 1];
  tickFromError[err - 1] = HAL_GetTick();
  return hasError;
}

static uint8_t checkCrash(uint8_t condition, CrashType crsh, uint16_t value, uint32_t timeToSave, uint8_t (*updatePred)(uint16_t, uint16_t),
                          uint8_t saveCondition) {
  HasErrorFlags hasError = 0;
  if (condition) {
    hasError = HasCrashes;
    if (general.screens.mainScrValues.crashes[crsh] == 0) {
      timeFromCrash[crsh - 1] = 0;
      tickFromCrash[crsh - 1] = HAL_GetTick();
      setCrash(crsh, value);
    } else if (timeFromCrash[crsh - 1] < timeToSave)
      setCrash(crsh, value);
    else if (saveCondition) {
      if (crsh == TempCrash && timeFromCrash[crsh - 1] < 60 * 60 * 1000) {
        if (updatePred(value, general.screens.mainScrValues.crashes[crsh]))
          general.screens.mainScrValues.crashes[crsh] = value;
        if (general.control.relayState == 1)
          DisableRelay();
        timeFromCrash[crsh - 1] += HAL_GetTick() - tickFromCrash[crsh - 1];
        tickFromCrash[crsh - 1] = HAL_GetTick();
        return hasError;
      }
      if ((crashesFlag & (1 << crsh)) != 0) {
        if (updatePred(value, general.screens.crashesScrValues.crashes[crsh].value)) {
          general.eeErrors.crashes[crsh - 1].value = value;
          general.screens.crashesScrValues.crashes[crsh].value = value;
        }
      } else {
        DisableRelay();
        crashesFlag |= 1 << crsh;
        hasError = HasCrashesToSave;
        saveCrash(crsh, value, &general.settings.currentDate);
      }
    }
  } else if ((crashesFlag & (1 << crsh)) != 0) {
    crashesFlag &= ~(1 << crsh);
  } else if (general.screens.mainScrValues.crashes[crsh] != 0 && general.control.relayState == 1) {
    resetCrash(crsh);
  }
  timeFromCrash[crsh - 1] += HAL_GetTick() - tickFromCrash[crsh - 1];
  tickFromCrash[crsh - 1] = HAL_GetTick();
  return hasError;
}

static uint8_t saveCondition(void) {
  if ((crashesFlag & ((1 << OvervoltageCrash) | (1 << TempCrash) | (1 << InTempSensCrash) | (1 << OutTempSensCrash))) != 0 ||
      (errorsFlag & (1 << ManualPowerOffError)) != 0)
    return false;
  return true;
}

static uint16_t currentDelay(void) {
  if (general.sensors.current / 10 >= (3 * general.settings.maxCurrent))
    return 0;
  else
    return 2000;
}

static uint16_t voltageDelay(void) {
  if (general.sensors.voltage >= (general.settings.maxCurrent + general.settings.maxCurrent / 5))
    return 0;
  else
    return 2000;
}

// TODO: добавить обработку остальных ошибок. В частности отдельно для температуры эксплуатации, амплитудного напряжения и отключения нагрузки. Возможно следует
// проверять все ошибки по дополнительному флагу, чтобы проверка для всех ошибок была через один и тот же промежуток времени (уменьшить количество проверок).
void StartRelayStateControlTask(void *argument) {
  uint32_t flags = 0;
  HasErrorFlags hasErrors = 0;
  uint8_t enableDelay = 0;
  DisableRelay();
  general.state.enableDelayMs = 5000;
  flags = osThreadFlagsWait(STARTUP, osFlagsWaitAny, osWaitForever);
  for (;;) {
    flags = osThreadFlagsWait(0x7FFFFFFF, osFlagsWaitAny, osWaitForever);
    for (uint8_t i = 0; i < 12; i++) {
      uint32_t flag = flags & (1 << i);
      if (flag == 0)
        continue;
      if (general.state.isRestarting) {
        general.state.isRestarting = 0;
        osDelay(250);
        break;
      }
      switch (flag) {
      case VOLTAGE_CURRENT_RMS:
        // TODO: Отключение питания. Срабатывает после 24 часов непрерывной работы
        /*if (general.sensors.voltage <= 170 && (errorsFlag & (1 << PowerOffError)) == 0) {
          DisableRelay();
          writeError16ToEE(PowerOffError, 1, &general.settings.currentDate, &general.settings.currentDate);
          saveEeErrors();
          errorsFlag |= 1 << PowerOffError;
        }*/

        // TODO: напряжение, ток, перенапряжение
        hasErrors |= checkError(general.sensors.voltage < general.settings.minVoltage || general.sensors.voltage >= general.settings.maxVoltage,
                                VoltageRmsError, general.sensors.voltage, voltageDelay(),
                                getGreaterLessPred(general.sensors.voltage, general.settings.minVoltage, general.settings.maxVoltage), saveCondition());
        hasErrors |= checkError(general.sensors.current / 10 >= general.settings.maxCurrent, CurrentError, general.sensors.current / 10, currentDelay(),
                                greaterPred, saveCondition());
        hasErrors |= checkCrash(general.sensors.voltage >= OVER_VOLTAGE, OvervoltageCrash, general.sensors.voltage, 0, greaterPred, 1);
        break;
      case VOLTAGE_AMP_OVERLOAD:
        // TODO: амплитуда напряжения
        if (!general.settings.voltageIrrAllowed) {
          hasErrors |= checkError(general.sensors.voltageAmp >= OVER_AMP_VOLTAGE, VoltageAmpError, general.sensors.voltageAmp, 0, greaterPred, saveCondition());
        }
        /*if (general.sensors.voltageAmp <= 150 && (errorsFlag & (1 << PowerOffError)) == 0) {
          DisableRelay();
          writeError16ToEE(PowerOffError, 1, &general.settings.currentDate, &general.settings.currentDate);
          saveEeErrors();
          errorsFlag |= 1 << PowerOffError;
          timeFromError[PowerOffError - 1] = 0;
          tickFromError[PowerOffError - 1] = HAL_GetTick();
        } else */
        if ((errorsFlag & (1 << PowerOffError)) != 0) {
          timeFromError[PowerOffError - 1] += HAL_GetTick() - tickFromError[PowerOffError - 1];
          tickFromError[PowerOffError - 1] = HAL_GetTick();
          if (timeFromError[PowerOffError - 1] >= 2000) {
            errorsFlag &= ~(1 << PowerOffError);
            memcpy(&(general.eeErrors.errors[PowerOffError - 1]), &tempPowerOffError, sizeof(err_record_t));
            if (tempPowerOffError.value == 0)
              general.eeErrors.errorsCount--;
            osThreadFlagsSet(flashTaskHandle, ERRORS_CHANGED);
          }
        }
        break;
      case CURRENT_AMP_OVERLOAD:
        // TODO: КЗ
        hasErrors |= checkCrash(general.sensors.currentAmp / 10 >= OVER_AMP_CURRENT, ShortCircuitCrash, general.sensors.currentAmp / 10, 0, greaterPred, 1);
        break;
      case VOLTAGE_FFT:
        // TODO: THD
        if (!general.settings.thdAllowed) {
          hasErrors |= checkError(general.screens.mainScrValues.thd >= general.settings.thdPercent, ThdError, general.screens.mainScrValues.thd, 2000,
                                  greaterPred, saveCondition());
        }
        break;
      case FREQ_MEASURE:
        // TODO: частота
        if (!general.settings.freqIrrAllowed) {
          hasErrors |=
              checkError(general.sensors.frequency / 10 >= FREQ_MAX || general.sensors.frequency / 10 < FREQ_MIN, ThdError, general.sensors.frequency / 10,
                         2000, getGreaterLessPred(general.sensors.frequency / 10, FREQ_MIN, FREQ_MAX), saveCondition());
        }
        break;
      case RELAY_EN_REQ:
        if (general.screens.mainScrValues.isPaused == 0) {
          if (hasErrors == 0 && general.state.enableDelayMs == 0) {
            if (errorsToSaveFlag) {
              for (int i = 1; i <= ERR_MAX_COUNT; i++) {
                uint32_t flag = errorsToSaveFlag & (1 << i);
                if (flag == 0)
                  continue;
                setDateTo((ErrorType)i, &general.settings.currentDate);
              }
              // TODO: запись в EEPROM
              osThreadFlagsSet(flashTaskHandle, ERRORS_CHANGED);
              errorsToSaveFlag = 0;
            }
            EnableRelay();
            // general.state.isRestarting = 0;
            general.state.enableDelayFlags &= ~ENABLE_RELAY_DELAY;
            if (errorsFlag & (1 << ManualPowerOffError)) // Для сохранения ошибки ручного отключения питания
              errorsFlag = 1 << ManualPowerOffError;
            else
              errorsFlag = 0;
            crashesFlag = 0;
            resetCrashes();
            resetErrors();
          } else {
            if (errorsFlag == (1 << ManualPowerOffError) || (hasErrors & HasCrashes) || enableDelay) {
              enableDelay = 0;
              general.state.enableDelayMs = 5000;
              resetCrashes();
            } else
              general.state.enableDelayMs = 0;
            general.state.isRestarting = 1;
            general.state.enableDelayFlags |= ENABLE_RELAY_DELAY;
          }
          hasErrors = 0;
        }
        break;
      case RELAY_DIS_REQ:
        DisableRelay();
        break;
      case RELAY_ENABLED:
        // TODO: пропуски при срабатывание силового реле
        if (general.control.relayState == 0) {
          hasErrors |= HasCrashes | HasCrashesToSave;
          setCrash(RelayCrash, 1);
          crashesFlag |= 1 << RelayCrash;
          saveCrash(RelayCrash, 1, &general.settings.currentDate);
        }
        general.state.isRestarting = 0;
        break;
      case RELAY_DISABLED:
        // TODO: пропуски при срабатывание силового реле
        if (general.control.relayState == 1) {
          hasErrors |= HasCrashes | HasCrashesToSave;
          setCrash(RelayCrash, 1);
          crashesFlag |= 1 << RelayCrash;
          saveCrash(RelayCrash, 1, &general.settings.currentDate);
        }
        general.screens.mainScrValues.errorChanged = 1;
        break;
      case SYS_TICK:
        // TODO: перегрев клеммы вход/выход, недопустимая t эксплуатации, обрыв датчика перегрева клеммы вход/выход, ручное отключение нагрузки
        hasErrors |= checkCrash(general.sensors.temp >= TEMP_CPU_MAX || general.sensors.temp < TEMP_CPU_MIN, TempCrash, general.sensors.temp + 256, 2000,
                                getGreaterLessPred(general.sensors.temp, TEMP_CPU_MIN, TEMP_CPU_MAX), 1);
        hasErrors |= checkCrash(general.sensors.inHeat < TEMP_SENS_MAX, InHeatCrash, general.sensors.inHeat + 1, 2000, greaterPred, saveCondition());
        hasErrors |= checkCrash(general.sensors.inHeat >= TEMP_SENS_CRASH, InTempSensCrash, general.sensors.inHeat + 1, 2000, greaterPred, saveCondition());
        hasErrors |= checkCrash(general.sensors.outHeat < TEMP_SENS_MAX, OutHeatCrash, general.sensors.outHeat + 1, 2000, greaterPred, saveCondition());
        hasErrors |= checkCrash(general.sensors.inHeat >= TEMP_SENS_CRASH, OutTempSensCrash, general.sensors.outHeat + 1, 2000, greaterPred, saveCondition());
        hasErrors |= checkError(general.screens.mainScrValues.isPaused == 1, ManualPowerOffError, 1, 2000, truePred, 1);
        if (!errorsFlag && !crashesFlag && general.control.relayState == 0 && general.screens.mainScrValues.isPaused == 0 && general.state.enableDelayMs == 0) {
          if (general.screens.mainScrValues.errors[0] != 0 || general.screens.mainScrValues.crashes[0] != 0)
            enableDelay = 1;
          else
            EnableRelayReq();
        } else {
          if (crashesFlag || (hasErrors & (HasCrashes | HasCrashesToSave))) {
            if (general.state.enableDelayFlags & ENABLE_RELAY_DELAY) {
              general.state.enableDelayFlags &= ~ENABLE_RELAY_DELAY;
              general.state.enableDelayMs = 0;
            }
          } else if (((hasErrors & HasErrorsToSave) || general.state.enableDelayMs != 0) && !errorsFlag) {
            if ((general.state.enableDelayFlags & ENABLE_RELAY_DELAY) == 0 && general.screens.mainScrValues.isPaused == 0) {
              general.state.enableDelayFlags |= ENABLE_RELAY_DELAY;
              general.screens.mainScrValues.errorChanged = 1;
            }
          }
          /*if (general.state.enableDelayMs == 0) {
            if ((general.state.enableDelayFlags & ENABLE_RELAY_DELAY) != 0) {
              general.state.enableDelayFlags &= ~ENABLE_RELAY_DELAY;
              general.screens.mainScrValues.errorChanged = 1;
            }
          }*/
          if (hasErrors & HasCrashesToSave) { // Есть краши на запись
            // TODO: запись в EEPROM
            hasErrors &= ~HasCrashesToSave;
            osThreadFlagsSet(flashTaskHandle, ERRORS_CHANGED);
          }
        }
        break;
      default:
        break;
      }
    }
  }
}

void StartRelayTask(void *argument) {
  uint32_t flags = 0;
  for (;;) {
    flags = osThreadFlagsWait(0x7FFFFFFF, osFlagsWaitAny, osWaitForever);
    if (flags & RELAY_ON) {
      HAL_GPIO_WritePin(RELAY_ON_GPIO_Port, RELAY_ON_Pin, GPIO_PIN_SET);
      general.control.relayState = 1;
      osDelay(100);
      general.state.relayOi = 0;
      osDelay(100);
      HAL_GPIO_WritePin(RELAY_ON_GPIO_Port, RELAY_ON_Pin, GPIO_PIN_RESET);
      if (general.state.relayOi == 0)
        general.control.relayState = 0;
      osThreadFlagsSet(relSCTaskHandle, RELAY_ENABLED);
    }
    if (flags & RELAY_OFF) {
      HAL_GPIO_WritePin(RELAY_OFF_GPIO_Port, RELAY_OFF_Pin, GPIO_PIN_SET);
      general.control.relayState = 0;
      osDelay(100);
      general.state.relayOi = 0;
      osDelay(100);
      HAL_GPIO_WritePin(RELAY_OFF_GPIO_Port, RELAY_OFF_Pin, GPIO_PIN_RESET);
      if (general.state.relayOi == 1)
        general.control.relayState = 1;
      osThreadFlagsSet(relSCTaskHandle, RELAY_DISABLED);
    }
    if (flags & RELAY_CHECK_AMP) {
      if (general.sensors.voltageAmp <= POWER_OFF_V_AMP && (errorsFlag & (1 << PowerOffError)) == 0) {
        HAL_GPIO_WritePin(RELAY_OFF_GPIO_Port, RELAY_OFF_Pin, GPIO_PIN_SET); // Выклюение без всяких проверок (быстрее)
        if (general.eeErrors.errors[PowerOffError - 1].value == 2)
          memcpy(&tempPowerOffError, &(general.eeErrors.errors[PowerOffError - 1]), sizeof(err_record_t));
        else if (general.eeErrors.errors[PowerOffError - 1].value == 0)
          general.eeErrors.errorsCount++;
        writeError16ToEE(PowerOffError, 1, &general.settings.currentDate, &general.settings.currentDate);
        saveEeErrors();
        if (general.eeTime.daysWork != 0)
          saveEeTime();
        errorsFlag |= 1 << PowerOffError;
        timeFromError[PowerOffError - 1] = 0;
        tickFromError[PowerOffError - 1] = HAL_GetTick();
        DisableRelay(); // Выключить ещё раз с проверками (для нормальной работы)
      }
    }
  }
}

void StartButtonsTask(void *argument) {
  uint32_t buttonTicks[3] = {0, 0, 0};
  uint32_t lastTicks = 0;
  uint32_t delta = 0;
  for (;;) {
    osDelay(27);
    general.control.idleTime += delta = xTaskGetTickCount() - lastTicks;
    if (HAL_GPIO_ReadPin(BUTTON_1_GPIO_Port, BUTTON_1_Pin) == 0) {
      buttonTicks[0] += delta;
      if (buttonTicks[0] > 50) {
        general.control.btnPressed |= (1 << 0);
        general.control.btnTimePressed[0] += delta;
        general.control.idleTime = 0;
      }
    } else {
      general.control.btnPressed &= ~(1 << 0);
      general.control.btnTimePressed[0] = 0;
      buttonTicks[0] = 0;
    }

    if (HAL_GPIO_ReadPin(BUTTON_2_GPIO_Port, BUTTON_2_Pin) == 0) {
      buttonTicks[1] += delta;
      if (buttonTicks[1] > 50) {
        general.control.btnPressed |= (1 << 1);
        general.control.btnTimePressed[1] += delta;
        general.control.idleTime = 0;
      }
    } else {
      general.control.btnPressed &= ~(1 << 1);
      general.control.btnTimePressed[1] = 0;
      buttonTicks[1] = 0;
    }

    if (HAL_GPIO_ReadPin(BUTTON_3_GPIO_Port, BUTTON_3_Pin) == 0) {
      buttonTicks[2] += delta;
      if (buttonTicks[2] > 50) {
        general.control.btnPressed |= (1 << 2);
        general.control.btnTimePressed[2] += delta;
        general.control.idleTime = 0;
      }
    } else {
      general.control.btnPressed &= ~(1 << 2);
      general.control.btnTimePressed[2] = 0;
      buttonTicks[2] = 0;
    }

    lastTicks = xTaskGetTickCount();

    switch (general.screens.currentScreen) {
    case 0:
      NavigateMainScreen();
      break;
    case 1:
      NavigateSettingsScreen();
      break;
    case 2:
      NavigateAfterErrorScreen();
      break;
    case 3:
      NavigateErrorScreen();
      break;
    }
    NavigateShared();
  }
}

void StartDisplayTask(void *argument) {
  HAL_TIM_PWM_Start(&LED_TIM_Handle, LED_TIM_Channel);
  DisplaySetBrightness(50);
  InitDisplay((void (*)(uint32_t)) & osDelay);

  InitMainScreen();
  InitAfterErrScreen();
  InitErrorsScreen();
  InitSetupScreen();
  InitScreenSaver();

  general.screens.lastScreen = 255;

  SetCSLow();
  FillBackground(*bgColors[general.settings.bgColorType]);
  SetCSHigh();

//  HAL_TIM_PWM_Start(&LED_TIM_Handle, LED_TIM_Channel);
//  DisplaySetBrightness(50);

  MX_SPI1_Init();
  W25qxx_Init();
  general.screens.currentScreen = 4;
  EnableScreenSaver();
  ShowScreenSaver((void (*)(uint32_t))osDelay, HAL_GetTick);
  HAL_SPI_MspDeInit(&FLASH_SPI_Handle);
  osThreadFlagsSet(flashTaskHandle, SPI_DONE);
  osDelay(250);
  osThreadFlagsSet(relSCTaskHandle, STARTUP);
  osDelay(250);
  general.screens.currentScreen = 0;
  for (;;) {
    if (general.control.relayState == 1) {
      if (general.screens.mainScrValues.isSleep && general.control.idleTime > 5000 && general.screens.currentScreen == 0)
        DisplaySetBrightness(0);
      else if (general.control.idleTime < 5000) {
        if (general.screens.mainScrValues.isSleep)
          DisplaySetBrightness(10);
        else
          DisplaySetBrightness(50);
      }
    } else
      DisplaySetBrightness(50);
    if (general.screens.lastScreen != general.screens.currentScreen && general.screens.lastScreen == 1)
      osThreadFlagsSet(flashTaskHandle, SETTINGS_CHANGED);
    switch (general.screens.currentScreen) {
    case 0:
      if (general.screens.lastScreen != general.screens.currentScreen) {
        EnableMainScrSet();
      } else
        UpdateMainScrSet(xTaskGetTickCount());
      break;
    case 1:
      if (general.screens.lastScreen != general.screens.currentScreen) {
        EnableSetupScrSet();
      } else
        UpdateSetupScrSet(xTaskGetTickCount());
      break;
    case 2:
      if (general.screens.lastScreen != general.screens.currentScreen)
        EnableAfterErrorScrSet();
      else
        UpdateAfterErrorScrSet();
      break;
    case 3:
      if (general.screens.lastScreen != general.screens.currentScreen)
        EnableErrorScrSet();
      else
        UpdateErrorScrSet();
      break;
    case 4:
      if (general.screens.lastScreen != general.screens.currentScreen)
        EnableScreenSaver();
      else {
        general.state.enableDelayMs = 10000;
        DisableRelay();
        resetSettings();
        general.state.isTimeDateChanged = 1;
        ShowScreenSaver((void (*)(uint32_t))osDelay, HAL_GetTick);
        eraseCrashes();
        eraseErrors();
        ee24_eraseChip();
        /*general.eeTime.daysWork = 0;
        general.eeTime.relayWorkTimeS = 0;*/
        saveEeErrors();
        saveEeSettings();
        saveEeTime();
        NVIC_SystemReset();
      }
    }
    osDelay(20);
  }
}

void StartRtcTask(void *argument) {
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  uint32_t day = HAL_RTCEx_BKUPRead(&RTC_Handle, RTC_BKP_DR1);
  uint32_t month = HAL_RTCEx_BKUPRead(&RTC_Handle, RTC_BKP_DR2);
  uint32_t year = HAL_RTCEx_BKUPRead(&RTC_Handle, RTC_BKP_DR3);

  sDate.Date = (day > 0 && day < 32) ? day : 1;
  sDate.Month = (month > 0 && month < 13) ? month : 1;
  sDate.Year = (year < 100) ? year : 20;

  uint8_t maxDay = GetMaxDay(sDate.Month, sDate.Year);
  if (maxDay < sDate.Date)
    sDate.Date = maxDay;

  HAL_RTC_SetDate(&RTC_Handle, &sDate, RTC_FORMAT_BIN);

  HAL_RTC_GetTime(&RTC_Handle, &sTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&RTC_Handle, &sDate, RTC_FORMAT_BIN);
  general.settings.currentDate.minutes = sTime.Minutes;
  general.settings.currentDate.hours = sTime.Hours;
  general.settings.currentDate.day = sDate.Date;
  general.settings.currentDate.month = sDate.Month;
  general.settings.currentDate.year = sDate.Year;

  osThreadFlagsSet(flashTaskHandle, RTC_DONE);

  uint16_t timeCur = 0, timeStart = 0, timeDur = 0;
  uint8_t lastSec = 0;

  for (;;) {
    if (general.state.isTimeDateChanged) {
      sTime.Seconds = 0;
      sTime.Minutes = general.settings.currentDate.minutes;
      sTime.Hours = general.settings.currentDate.hours;
      HAL_RTC_SetTime(&RTC_Handle, &sTime, RTC_FORMAT_BIN);

      maxDay = GetMaxDay(general.settings.currentDate.month, general.settings.currentDate.year);
      if (maxDay < general.settings.currentDate.day)
        general.settings.currentDate.day = maxDay;
      sDate.Date = general.settings.currentDate.day;
      sDate.Month = general.settings.currentDate.month;
      sDate.Year = general.settings.currentDate.year;
      HAL_RTC_SetDate(&RTC_Handle, &sDate, RTC_FORMAT_BIN);

      general.state.isTimeDateChanged = 0;
    } else {
      HAL_RTC_GetTime(&RTC_Handle, &sTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&RTC_Handle, &sDate, RTC_FORMAT_BIN);
      general.settings.currentDate.minutes = sTime.Minutes;
      general.settings.currentDate.hours = sTime.Hours;
      general.settings.currentDate.day = sDate.Date;
      general.settings.currentDate.month = sDate.Month;
      general.settings.currentDate.year = sDate.Year;
      HAL_RTCEx_BKUPWrite(&RTC_Handle, RTC_BKP_DR1, sDate.Date);
      HAL_RTCEx_BKUPWrite(&RTC_Handle, RTC_BKP_DR2, sDate.Month);
      HAL_RTCEx_BKUPWrite(&RTC_Handle, RTC_BKP_DR3, sDate.Year);
    }
    timeCur = general.settings.currentDate.hours * 60 + general.settings.currentDate.minutes;
    timeStart = general.settings.lightTimeH * 60 + general.settings.lightTimeM;
    timeDur = general.settings.lightDurH * 60;
    if (timeStart + timeDur < 24 * 60) {
      if (timeCur < timeStart || timeCur >= timeStart + timeDur)
        general.screens.mainScrValues.isSleep = 1;
      else
        general.screens.mainScrValues.isSleep = 0;
    } else {
      if (timeCur < timeStart && timeCur >= (timeStart + timeDur) % (24 * 60))
        general.screens.mainScrValues.isSleep = 1;
      else
        general.screens.mainScrValues.isSleep = 0;
    }
    if (sTime.Seconds == 0)
      general.eeTime.relayWorkTimeS += 1;
    else
      general.eeTime.relayWorkTimeS += sTime.Seconds - lastSec;
    lastSec = sTime.Seconds;
    if (general.eeTime.relayWorkTimeS % (60 * 60) == 0) {
      general.eeTime.daysWork = general.eeTime.relayWorkTimeS / 60 / 60 / 24;
      osThreadFlagsSet(flashTaskHandle, TIME_CHANGED);
    }
    osDelay(500);
  }
}

void EnableRelayReq(void) { osThreadFlagsSet(relSCTaskHandle, RELAY_EN_REQ); }

void DisableRelayReq(void) { osThreadFlagsSet(relSCTaskHandle, RELAY_DIS_REQ); }

void ProcessSysTickDelays(void) {
  if (general.state.enableDelayMs > 0 && (general.state.enableDelayFlags & ENABLE_RELAY_DELAY))
    general.state.enableDelayMs--;
}