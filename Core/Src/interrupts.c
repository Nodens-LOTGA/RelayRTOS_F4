#include "interrupts.h"
#include "cmsis_os2.h"
#include "settings.h"
#include "tasks.h"
#include "utils.h"

extern osThreadId_t relTaskHandle;
extern osThreadId_t relSCTaskHandle;

uint8_t icState = 0;
uint32_t icCnt1 = 0;
uint32_t icCnt2 = 0;
uint8_t icOvf = 0;
uint64_t icTicks = 0;
uint8_t measureCnt = 0;
int32_t avgVoltage = 0;
int32_t avgCurrent = 0;
int32_t avgTemp = 0;
int32_t avgInHeat = 0;
int32_t avgOutHeat = 0;
uint8_t lastFreq = 0;
uint8_t bufNum = 0;

void MEASURE_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == FREERTOS_TIM) {
    ProcessSysTickDelays();
    if (osKernelGetState() == osKernelRunning)
      SetUpdateTickEvent();
  } else if (htim->Instance == ADC_TIM) {
    if (osKernelGetState() == osKernelRunning) {
      if (general.sensors.values_ptr < FFT_SIZE) {
        __disable_irq();
        avgVoltage += general.sensors.adc_value[0];
        avgCurrent += general.sensors.adc_value[1];
        avgInHeat += general.sensors.adc_value[2];
        avgOutHeat += general.sensors.adc_value[3];
        avgTemp += general.sensors.adc_value[4];
        if (general.sensors.adc_value[0] > general.sensors.maxV)
          general.sensors.maxV = general.sensors.adc_value[0];
        if (general.sensors.adc_value[1] > general.sensors.maxC)
          general.sensors.maxC = general.sensors.adc_value[1];
        if (measureCnt == MEASURE_COUNT) {
          general.sensors.voltage_values[general.sensors.values_ptr] = avgVoltage / MEASURE_COUNT;
          general.sensors.current_values[general.sensors.values_ptr] = avgCurrent / MEASURE_COUNT;
          general.sensors.fft_voltage_in[FFT_SIZE * bufNum + general.sensors.values_ptr] = (float)(general.sensors.voltage_values[general.sensors.values_ptr]);
          if ((general.sensors.values_ptr + 1) % (FFT_SIZE / PERIODS_COUNT) == 0) {
            general.sensors.temp = (1430 - (avgTemp * VDD_VALUE) / (MEASURE_COUNT * 4095)) / 4.3 + 25;
            general.sensors.inHeat = avgInHeat / MEASURE_COUNT;
            general.sensors.outHeat = avgOutHeat / MEASURE_COUNT;
            general.sensors.voltageAmp = RECOUNT_VOLTAGE((general.sensors.maxV - general.settings.voltageZeroAdc) * 10);
            general.sensors.currentAmp = RECOUNT_CURRENT((general.sensors.maxC - general.settings.currentZeroAdc) * 10);
            if (general.sensors.voltageAmp <= POWER_OFF_V_AMP) {
              // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
              osThreadFlagsSet(relTaskHandle, RELAY_CHECK_AMP);
            }
            osThreadFlagsSet(relSCTaskHandle, VOLTAGE_AMP_OVERLOAD | CURRENT_AMP_OVERLOAD);
          }
          general.sensors.values_ptr++;
          avgVoltage = 0;
          avgCurrent = 0;
          avgTemp = 0;
          avgInHeat = 0;
          avgOutHeat = 0;
          measureCnt = 0;
        }
        measureCnt++;
        __enable_irq();
      } else if (general.sensors.values_ptr == FFT_SIZE) {
        general.sensors.maxV = 0;
        general.sensors.maxC = 0;
        general.sensors.values_ptr = 0;
        general.sensors.values_bufNum = bufNum;
        bufNum = (bufNum + 1) % 2;
        /* if (general.sensors.frequency != lastFreq) {
           calculateTimerReg((PERIODS_COUNT * 10000000) / (general.sensors.frequency * FFT_SIZE * MEASURE_COUNT), ADC_TIM, &ADC_TIM->PSC, &ADC_TIM->ARR);
           lastFreq = general.sensors.frequency;
         }*/
        SetAdcCaptureTimerEvent();
      }
    }
  } else if (htim->Instance == FREQUENCY_TIM) {
    icOvf++;
  }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if (icState == 0) {
    icCnt1 = FREQUENCY_TIM->FREQUENCY_TIM_CCR;
    icOvf = 0;
    icState = 1;
  } else if (icState == 1) {
    icCnt2 = FREQUENCY_TIM->FREQUENCY_TIM_CCR;
    icTicks = (icCnt2 + (icOvf * (uint64_t)(FREQUENCY_TIM->ARR + 1))) - icCnt1;
    icOvf = 0;
    icState = 2;
  } else if (icState == 2) {
    icCnt1 = FREQUENCY_TIM->FREQUENCY_TIM_CCR;
    icTicks += (icCnt1 + (icOvf * (uint64_t)(FREQUENCY_TIM->ARR + 1))) - icCnt2;
    if (icTicks > 10000 && icTicks < (FREQUENCY_TIM->ARR + 1) * 2) {
      general.sensors.frequency = ((uint64_t)(SystemCoreClock * 10 / (36 * icTicks)));
    }
    icState = 0;
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
  case VOLTAGE_DETECT_Pin:
    general.state.relayOi = 1;
    break;

  case AMPLITUDE_DETECT_Pin:
    break;
  }
}