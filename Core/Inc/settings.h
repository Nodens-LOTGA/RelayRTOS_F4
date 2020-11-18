#ifndef SETTINGS_H
#define SETTINGS_H

#include "colors.h"
#include "main.h"

#define TRUE 1
#define FALSE 0

#define BTN_COUNT 3
#define BTN_SHORT_DELAY 200
#define BTN_LONG_DELAY 500

#define ERR_MAX_COUNT 7
#define CRASH_MAX_COUNT 8

#define MEASURE_COUNT 5
#define FFT_SIZE 256
#define PERIODS_COUNT 10
#define OVER_VOLTAGE 450
#define POWER_OFF_V_AMP 150
#define OVER_AMP_CURRENT 100
#define OVER_AMP_VOLTAGE 630
#define FREQ_MIN 47
#define FREQ_MAX 53
#define TEMP_CPU_MIN -15
#define TEMP_CPU_MAX 60
#define TEMP_SENS_MAX 3127   // Значения АЦП
#define TEMP_SENS_CRASH 4090 // Значения АЦП

#define RECOUNT_VOLTAGE(x) (x * 4 / 105) // 11 / 290 или 4 / 105
#define RECOUNT_CURRENT(x) (x * 5 / 106) // 9 / 77 или 7 / 60

typedef struct {
  uint8_t minutes;
  uint8_t hours;
  uint8_t day;
  uint8_t month;
  uint8_t year;
} datetime_t;

typedef struct {
  uint16_t minutes;
  uint16_t hours;
  uint16_t day;
  uint16_t month;
  uint16_t year;
} datetime16_t;

typedef struct {
  uint16_t adc_value[5];
  uint16_t frequency;
  uint16_t voltage;
  uint16_t voltageAmp;
  uint16_t maxV;
  uint16_t current;
  uint16_t currentAmp;
  uint16_t maxC;
  uint16_t values_ptr;
  uint8_t values_bufNum;
  uint16_t sin;
  uint16_t harm;
  int16_t temp;
  int16_t inHeat;
  int16_t outHeat;
  uint16_t voltage_values[FFT_SIZE];
  uint16_t current_values[FFT_SIZE];
  float fft_voltage_in[2 * FFT_SIZE];
  float fft_voltage_out[FFT_SIZE];
} sensors_t;

typedef struct {
  uint16_t maxVoltage;
  uint16_t minVoltage;
  uint16_t maxCurrent;
  uint8_t loadType;
  datetime16_t currentDate;
  uint8_t lightTimeH;
  uint8_t lightTimeM;
  uint8_t lightDurH;
  BgColors bgColorType;
  uint16_t startDelayS;
  uint8_t thdAllowed;
  uint8_t thdPercent;
  uint8_t freqIrrAllowed;
  uint8_t voltageIrrAllowed;
  uint16_t currentZeroAdc;
  uint16_t voltageZeroAdc;
} settings_t;

typedef enum { ENABLE_RELAY_DELAY = 1 } DelaysFlags;

typedef struct {
  uint32_t enableDelayMs;
  uint32_t enableDelayFlags;
  uint8_t errorDelays[ERR_MAX_COUNT];
  uint8_t isRestarting;
  uint8_t relayOi;
  uint8_t isTimeDateChanged;
  uint8_t recalcAdcZero;
} state_t;

typedef struct {
  datetime_t fromDate;
  datetime_t toDate;
  uint16_t value;
} err_record_t;

typedef struct {
  datetime_t date;
  uint16_t value;
} crash_record_t;

typedef struct {
  datetime16_t fromDate;
  datetime16_t toDate;
  uint16_t value;
} err_record16_t;

typedef struct {
  datetime16_t date;
  uint16_t value;
} crash_record16_t;

typedef struct {
  uint8_t errorsCount;
  uint8_t errorsOrder[ERR_MAX_COUNT];
  uint8_t crashesCount;
  uint8_t crashesOrder[CRASH_MAX_COUNT];
  err_record_t errors[ERR_MAX_COUNT];
  crash_record_t crashes[CRASH_MAX_COUNT];
} ee_errors_t;

typedef struct {
  uint32_t relayWorkTimeS;
  uint16_t daysWork;
} ee_time_t;

typedef struct {
  uint16_t voltageAmp;
  uint16_t voltageRms;
  uint16_t currentInteger;
  uint16_t currentFract;
  uint16_t freqInteger;
  uint16_t freqFract;
  uint16_t thd;
  uint16_t kWInteger;
  uint16_t kWFract;
  uint16_t crashes[CRASH_MAX_COUNT + 1]; // crashes[CrashType] = error value (uint32_t). crashes[0] = count;
  uint16_t errors[ERR_MAX_COUNT + 1];    // Same
  uint8_t errorChanged;
  uint8_t isPaused;
  uint16_t delayS;
  uint16_t delayM;
  uint16_t delayH;
  uint8_t isSleep;
  uint16_t temp;
} main_val_t;

typedef enum { OvervoltageCrash = 1, ShortCircuitCrash, TempCrash, InHeatCrash, OutHeatCrash, InTempSensCrash, OutTempSensCrash, RelayCrash } CrashType;

typedef struct {
  crash_record16_t currentCrash;
  crash_record16_t crashes[CRASH_MAX_COUNT + 1]; // The index is of CrashType. Starts from 1. crashes[0].value = count
  uint8_t crashesOrder[CRASH_MAX_COUNT];         // crashesOrder[0] is the last occured
} crashes_t;

typedef enum { ManualPowerOffError = 1, PowerOffError, VoltageRmsError, ThdError, CurrentError, VoltageAmpError, FreqError } ErrorType;

typedef struct {
  err_record16_t currentError;
  err_record16_t errors[ERR_MAX_COUNT + 1]; // The index is of ErrorType. Starts from 1. errors[0].value = count
  uint8_t errorsOrder[ERR_MAX_COUNT];       // errorsOrder[0] is the last occured error, [1] is the 2nd...
} errors_t;

typedef struct {
  uint8_t selectedEl;
  uint16_t itemNumber;
  uint16_t voltage;
  uint16_t current;
  uint16_t kWInteger;
  uint16_t kWFract;

  uint16_t setupTimeHours;
  uint16_t setupTimeMinutes;
  uint16_t setupDateDay;
  uint16_t setupDateMonth;
  uint16_t setupDateYear;

  uint16_t lightTimeHours;
  uint16_t lightTimeMinutes;

  uint16_t lightEnDuration;
  uint16_t lightDisDuration;
  uint16_t thdPercent;
  uint16_t delaySeconds;
} setup_val_t;

typedef struct {
  uint8_t btnPressed;
  uint32_t btnTimePressed[BTN_COUNT];
  uint8_t relayState;
  uint32_t idleTime;
} control_t;

typedef struct {
  main_val_t mainScrValues;
  crashes_t crashesScrValues;
  errors_t errorsScrValues;
  setup_val_t setupScrValues;
  uint8_t currentScreen;
  uint8_t currentScreenState;
  uint8_t lastState;
  uint8_t lastScreen;
} screens_t;

typedef struct {
  screens_t screens;
  sensors_t sensors;
  control_t control;
  settings_t settings;
  ee_errors_t eeErrors;
  ee_time_t eeTime;
  state_t state;
} general_t;

extern general_t general;

void setDateTo(ErrorType err, const datetime16_t *const date);
// Запись ошибки в экран ошибок и структуру EEPROM
void saveError(ErrorType err, uint16_t value, const datetime16_t *const from, const datetime16_t *const to);
// Добавление ошибки для отображения на главном экране
void setError(ErrorType err, uint16_t value);
// Удаление ошибок с экрана ошибок и структуры EEPROM
void eraseErrors(void);
// Очистка ошибки с главного экрана
void resetError(ErrorType err);
// Очистка всех ошибок с главного экрана
void resetErrors(void);
// Чтение ошибок из eeErrors
void loadErrors(void);

// Запись краша в экран ошибок и структуру EEPROM
void saveCrash(CrashType crsh, uint16_t value, const datetime16_t *const date);
// Добавление краша для отображения на главном экране
void setCrash(CrashType crsh, uint16_t value);
// Удаление крашей с экрана ошибок и структуры EEPROM
void eraseCrashes(void);
// Очистка краша с главного экрана
void resetCrash(CrashType crsh);
// Очистка всех крашей с главного экрана
void resetCrashes(void);
// Чтение крашей из eeErrors
void loadCrashes(void);

void writeError16ToEE(ErrorType err, uint16_t value, const datetime16_t *const from, const datetime16_t *const to);
void writeCrash16ToEE(CrashType crsh, uint16_t value, const datetime16_t *const date);

void loadEeErrors(void);
void saveEeErrors(void);
void loadEeSettings(void);
void saveEeSettings(void);
void loadEeTime(void);
void saveEeTime(void);

void resetSettings(void);

#endif // SETTINGS_H
