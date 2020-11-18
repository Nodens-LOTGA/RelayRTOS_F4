#ifndef FUNCTIONS_H
#define FUNCTIONS_H

typedef enum {
  VOLTAGE_CURRENT_RMS = 1 << 1,
  VOLTAGE_AMP_OVERLOAD = 1 << 2,
  CURRENT_AMP_OVERLOAD = 1 << 3,
  VOLTAGE_FFT = 1 << 4,
  FREQ_MEASURE = 1 << 5,
  RELAY_EN_REQ = 1 << 6,
  RELAY_DIS_REQ = 1 << 7,
  RELAY_ENABLED = 1 << 8,
  RELAY_DISABLED = 1 << 9,
  SYS_TICK = 1 << 10,
  STARTUP = 1 << 11
} UpdateFlags;

typedef enum {
  RELAY_ON = 1,
  RELAY_OFF = 2,
  RELAY_CHECK_AMP = 4
} RelayFlags;

typedef enum {
  SPI_DONE = 1,
  SETTINGS_CHANGED = 2,
  ERRORS_CHANGED = 4,
  RTC_DONE = 8,
  TIME_CHANGED = 16
} FlashFlags;

typedef enum {
  START_FFT = 1
} FftFlags;

typedef enum {
  START_ADC = 1
} AdcFlags;

void SetAdcCaptureTimerEvent(void);
void StartAdcCaptureTask(void *argument);
void StartFftTask(void *argument);
void StartFlashTask(void *argument);
void SetUpdateTickEvent(void);
void StartRelayStateControlTask(void *argument);
void StartRelayTask(void *argument);
void StartButtonsTask(void *argument);
void StartDisplayTask(void *argument);
void StartRtcTask(void *argument);
void EnableRelayReq(void);
void DisableRelayReq(void);
void ProcessSysTickDelays(void);

#endif // FUNCTIONS_H
