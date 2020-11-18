#include "settings.h"
#include "ee24.h"

#define ALIGN(offset, align) offset + ((align - (offset % align) % align))

#define EE_ERRORS_OFFSET 0x00
#define EE_SETTINGS_OFFSET ALIGN(sizeof(ee_errors_t), 16)
#define EE_TIME_OFFSET ALIGN(EE_SETTINGS_OFFSET + sizeof(settings_t), 16)

void setDateTo(ErrorType err, const datetime16_t *const date) {
  general.screens.errorsScrValues.errors[err].toDate = *date;
  general.eeErrors.errors[err - 1].toDate.day = (uint8_t)date->day;
  general.eeErrors.errors[err - 1].toDate.month = (uint8_t)date->month;
  general.eeErrors.errors[err - 1].toDate.year = (uint8_t)date->year;
  general.eeErrors.errors[err - 1].toDate.minutes = (uint8_t)date->minutes;
  general.eeErrors.errors[err - 1].toDate.hours = (uint8_t)date->hours;
}

void writeError16ToEE(ErrorType err, uint16_t value, const datetime16_t *const from, const datetime16_t *const to) {
  general.eeErrors.errors[err - 1].fromDate.day = (uint8_t)from->day;
  general.eeErrors.errors[err - 1].fromDate.month = (uint8_t)from->month;
  general.eeErrors.errors[err - 1].fromDate.year = (uint8_t)from->year;
  general.eeErrors.errors[err - 1].fromDate.minutes = (uint8_t)from->minutes;
  general.eeErrors.errors[err - 1].fromDate.hours = (uint8_t)from->hours;
  general.eeErrors.errors[err - 1].toDate.day = (uint8_t)to->day;
  general.eeErrors.errors[err - 1].toDate.month = (uint8_t)to->month;
  general.eeErrors.errors[err - 1].toDate.year = (uint8_t)to->year;
  general.eeErrors.errors[err - 1].toDate.minutes = (uint8_t)to->minutes;
  general.eeErrors.errors[err - 1].toDate.hours = (uint8_t)to->hours;
  general.eeErrors.errors[err - 1].value = (uint8_t)value;
}
void writeCrash16ToEE(CrashType crsh, uint16_t value, const datetime16_t *const date) {
  general.eeErrors.crashes[crsh - 1].date.day = (uint8_t)date->day;
  general.eeErrors.crashes[crsh - 1].date.month = (uint8_t)date->month;
  general.eeErrors.crashes[crsh - 1].date.year = (uint8_t)date->year;
  general.eeErrors.crashes[crsh - 1].date.minutes = (uint8_t)date->minutes;
  general.eeErrors.crashes[crsh - 1].date.hours = (uint8_t)date->hours;
  general.eeErrors.crashes[crsh - 1].value = (uint8_t)value;
}

void saveError(ErrorType err, uint16_t value, const datetime16_t *const from, const datetime16_t *const to) {
  uint8_t errPos = 0;
  for (int i = 0; i < ERR_MAX_COUNT; i++) {
    if (general.screens.errorsScrValues.errorsOrder[i] == err || general.screens.errorsScrValues.errorsOrder[i] == 0) {
      general.screens.errorsScrValues.errorsOrder[i] = 0;
      general.eeErrors.errorsOrder[i] = 0;
      errPos = i;
      break;
    }
  }
  for (int i = errPos; i > 0; i--) {
    general.screens.errorsScrValues.errorsOrder[i] = general.screens.errorsScrValues.errorsOrder[i - 1];
    general.eeErrors.errorsOrder[i] = general.screens.errorsScrValues.errorsOrder[i - 1];
  }
  if (general.screens.errorsScrValues.errors[err].value == 0)
    general.screens.errorsScrValues.errors[0].value++;
  general.screens.errorsScrValues.errors[err].value = value;
  general.screens.errorsScrValues.errors[err].fromDate = *from;
  general.screens.errorsScrValues.errors[err].toDate = *to;
  general.screens.errorsScrValues.errorsOrder[0] = err;
  general.eeErrors.errorsOrder[0] = err;
  general.eeErrors.errorsCount = general.screens.errorsScrValues.errors[0].value;
  general.screens.mainScrValues.errorChanged = 1;

  writeError16ToEE(err, value, from, to);
}
void setError(ErrorType err, uint16_t value) {
  if (general.screens.mainScrValues.errors[err] == value)
    return;
  if (general.screens.mainScrValues.errors[err] == 0)
    general.screens.mainScrValues.errors[0]++;
  general.screens.mainScrValues.errors[err] = value;
  general.screens.mainScrValues.errorChanged = 1;
}
void eraseErrors(void) {
  for (int i = 0; i < ERR_MAX_COUNT; i++) {
    general.screens.errorsScrValues.errorsOrder[i] = 0;
    general.eeErrors.errorsOrder[i] = 0;
  }
  for (int i = 1; i < ERR_MAX_COUNT + 1; i++) {
    general.screens.errorsScrValues.errors[i].value = 0;
    general.screens.errorsScrValues.errors[i].fromDate.day = 0;
    general.screens.errorsScrValues.errors[i].fromDate.month = 0;
    general.screens.errorsScrValues.errors[i].fromDate.year = 0;
    general.screens.errorsScrValues.errors[i].fromDate.minutes = 0;
    general.screens.errorsScrValues.errors[i].fromDate.hours = 0;
    general.screens.errorsScrValues.errors[i].toDate.day = 0;
    general.screens.errorsScrValues.errors[i].toDate.month = 0;
    general.screens.errorsScrValues.errors[i].toDate.year = 0;
    general.screens.errorsScrValues.errors[i].toDate.minutes = 0;
    general.screens.errorsScrValues.errors[i].toDate.hours = 0;
    writeError16ToEE(i, 0, &general.screens.errorsScrValues.errors[i].fromDate, &general.screens.errorsScrValues.errors[i].toDate);
  }
  general.screens.errorsScrValues.errors[0].value = 0;
  general.eeErrors.errorsCount = 0;
}
void resetError(ErrorType err) {
  if (general.screens.mainScrValues.errors[err] != 0) {
    general.screens.mainScrValues.errors[0]--;
    general.screens.mainScrValues.errors[err] = 0;
    general.screens.mainScrValues.errorChanged = 1;
  }
}
void resetErrors(void) {
  for (int i = 0; i < ERR_MAX_COUNT + 1; i++)
    general.screens.mainScrValues.errors[i] = 0;
  general.screens.mainScrValues.errorChanged = 1;
}
void loadErrors(void) {
  for (int i = 0; i < ERR_MAX_COUNT; i++)
    general.screens.errorsScrValues.errorsOrder[i] = general.eeErrors.errorsOrder[i];
  for (int i = 1; i < ERR_MAX_COUNT + 1; i++) {
    general.screens.errorsScrValues.errors[i].value = general.eeErrors.errors[i - 1].value;
    general.screens.errorsScrValues.errors[i].fromDate.day = general.eeErrors.errors[i - 1].fromDate.day;
    general.screens.errorsScrValues.errors[i].fromDate.month = general.eeErrors.errors[i - 1].fromDate.month;
    general.screens.errorsScrValues.errors[i].fromDate.year = general.eeErrors.errors[i - 1].fromDate.year;
    general.screens.errorsScrValues.errors[i].fromDate.minutes = general.eeErrors.errors[i - 1].fromDate.minutes;
    general.screens.errorsScrValues.errors[i].fromDate.hours = general.eeErrors.errors[i - 1].fromDate.hours;
    general.screens.errorsScrValues.errors[i].toDate.day = general.eeErrors.errors[i - 1].toDate.day;
    general.screens.errorsScrValues.errors[i].toDate.month = general.eeErrors.errors[i - 1].toDate.month;
    general.screens.errorsScrValues.errors[i].toDate.year = general.eeErrors.errors[i - 1].toDate.year;
    general.screens.errorsScrValues.errors[i].toDate.minutes = general.eeErrors.errors[i - 1].toDate.minutes;
    general.screens.errorsScrValues.errors[i].toDate.hours = general.eeErrors.errors[i - 1].toDate.hours;
  }
  general.screens.errorsScrValues.errors[0].value = general.eeErrors.errorsCount;
  if (general.screens.errorsScrValues.errors[PowerOffError].value == 1) { // 1 - первый раз, 2 - последующие
    saveError(PowerOffError, 2, &general.screens.errorsScrValues.errors[PowerOffError].fromDate, &general.settings.currentDate);
  }
}

void saveCrash(CrashType crsh, uint16_t value, const datetime16_t *const date) {
  uint8_t errPos = 1;
  for (int i = 1; i < CRASH_MAX_COUNT; i++) {
    if (general.screens.crashesScrValues.crashesOrder[i] == crsh || general.screens.crashesScrValues.crashesOrder[i] == 0) {
      general.screens.crashesScrValues.crashesOrder[i] = 0;
      general.eeErrors.crashesOrder[i] = 0;
      errPos = i;
      break;
    }
  }
  for (int i = errPos; i > 0; i--) {
    general.screens.crashesScrValues.crashesOrder[i] = general.screens.crashesScrValues.crashesOrder[i - 1];
    general.eeErrors.crashesOrder[i] = general.screens.crashesScrValues.crashesOrder[i - 1];
  }
  if (general.screens.crashesScrValues.crashes[crsh].value == 0)
    general.screens.crashesScrValues.crashes[0].value++;
  general.screens.crashesScrValues.crashes[crsh].value = value;
  general.screens.crashesScrValues.crashes[crsh].date = *date;
  general.screens.crashesScrValues.crashesOrder[1] = crsh;
  general.eeErrors.crashesOrder[1] = crsh;
  general.eeErrors.crashesCount = general.screens.crashesScrValues.crashes[0].value;
  general.screens.mainScrValues.errorChanged = 1;

  writeCrash16ToEE(crsh, value, date);
}
void setCrash(CrashType crsh, uint16_t value) {
  switch (crsh) {
  case OvervoltageCrash:
    return;
  default:
    break;
  }
  if (general.screens.mainScrValues.crashes[crsh] == value)
    return;
  if (general.screens.mainScrValues.crashes[crsh] == 0)
    general.screens.mainScrValues.crashes[0]++;
  general.screens.mainScrValues.crashes[crsh] = value;
  general.screens.mainScrValues.errorChanged = 1;
}
void eraseCrashes(void) {
  general.eeErrors.crashesOrder[0] = 0;
  for (int i = 1; i < CRASH_MAX_COUNT; i++) {
    general.screens.crashesScrValues.crashesOrder[i] = 0;
    general.eeErrors.crashesOrder[i] = 0;
  }
  general.screens.crashesScrValues.crashes[0].value = 0;
  if (general.screens.crashesScrValues.crashes[OvervoltageCrash].value != 0) {
    general.screens.crashesScrValues.crashes[0].value++;
    general.screens.crashesScrValues.crashesOrder[1] = OvervoltageCrash;
    general.eeErrors.crashesOrder[1] = OvervoltageCrash;
  }
  if (general.screens.crashesScrValues.crashes[ShortCircuitCrash].value != 0) {
    general.screens.crashesScrValues.crashes[0].value++;
    general.screens.crashesScrValues.crashesOrder[2] = ShortCircuitCrash;
    general.eeErrors.crashesOrder[2] = ShortCircuitCrash;
  }
  if (general.screens.crashesScrValues.crashes[TempCrash].value != 0) {
    general.screens.crashesScrValues.crashes[0].value++;
    general.screens.crashesScrValues.crashesOrder[3] = TempCrash;
    general.eeErrors.crashesOrder[3] = TempCrash;
  }
  for (int i = 3; i < CRASH_MAX_COUNT + 1; i++) {
    general.screens.crashesScrValues.crashes[i].value = 0;
    general.screens.crashesScrValues.crashes[i].date.day = 0;
    general.screens.crashesScrValues.crashes[i].date.month = 0;
    general.screens.crashesScrValues.crashes[i].date.year = 0;
    general.screens.crashesScrValues.crashes[i].date.minutes = 0;
    general.screens.crashesScrValues.crashes[i].date.hours = 0;
    writeCrash16ToEE(i, 0, &general.screens.crashesScrValues.crashes[i].date);
  }
  general.eeErrors.crashesCount = general.screens.crashesScrValues.crashes[0].value;
}
void resetCrash(CrashType crsh) {
  if (general.screens.mainScrValues.crashes[crsh] != 0) {
    general.screens.mainScrValues.crashes[0]--;
    general.screens.mainScrValues.crashes[crsh] = 0;
    general.screens.mainScrValues.errorChanged = 1;
  }
}
void resetCrashes(void) {
  for (int i = 0; i < CRASH_MAX_COUNT + 1; i++)
    general.screens.mainScrValues.crashes[i] = 0;
  general.screens.mainScrValues.errorChanged = 1;
}
void loadCrashes(void) {
  for (int i = 0; i < CRASH_MAX_COUNT; i++)
    general.screens.crashesScrValues.crashesOrder[i] = general.eeErrors.crashesOrder[i];
  for (int i = 1; i < CRASH_MAX_COUNT + 1; i++) {
    general.screens.crashesScrValues.crashes[i].value = general.eeErrors.crashes[i - 1].value;
    general.screens.crashesScrValues.crashes[i].date.day = general.eeErrors.crashes[i - 1].date.day;
    general.screens.crashesScrValues.crashes[i].date.month = general.eeErrors.crashes[i - 1].date.month;
    general.screens.crashesScrValues.crashes[i].date.year = general.eeErrors.crashes[i - 1].date.year;
    general.screens.crashesScrValues.crashes[i].date.minutes = general.eeErrors.crashes[i - 1].date.minutes;
    general.screens.crashesScrValues.crashes[i].date.hours = general.eeErrors.crashes[i - 1].date.hours;
  }
  general.screens.crashesScrValues.crashes[0].value = general.eeErrors.crashesCount;
}

void loadEeErrors(void) { ee24_read(EE_ERRORS_OFFSET, (uint8_t *)&general.eeErrors, sizeof(ee_errors_t), 1000); }

void saveEeErrors(void) { ee24_write(EE_ERRORS_OFFSET, (uint8_t *)&general.eeErrors, sizeof(ee_errors_t), 1000); }

void loadEeSettings(void) { ee24_read(EE_SETTINGS_OFFSET, (uint8_t *)&general.settings, sizeof(settings_t), 1000); }

void saveEeSettings(void) { ee24_write(EE_SETTINGS_OFFSET, (uint8_t *)&general.settings, sizeof(settings_t), 1000); }

void loadEeTime(void) { ee24_read(EE_TIME_OFFSET, (uint8_t *)&general.eeTime, sizeof(ee_time_t), 1000); }

void saveEeTime(void) { ee24_write(EE_TIME_OFFSET, (uint8_t *)&general.eeTime, sizeof(ee_time_t), 1000); }

void resetSettings(void) {
  general.settings.maxVoltage = 250;
  general.settings.minVoltage = 180;
  general.settings.maxCurrent = 25;
  general.settings.loadType = 0;
  general.settings.currentDate.hours = 12;
  general.settings.currentDate.minutes = 0;
  general.settings.currentDate.day = 1;
  general.settings.currentDate.month = 1;
  general.settings.currentDate.year = 20;
  general.settings.lightTimeH = 18;
  general.settings.lightTimeM = 00;
  general.settings.lightDurH = 15;
  general.settings.bgColorType = BgWhiteColor;
  general.settings.startDelayS = 50;
  general.settings.thdAllowed = 0;
  general.settings.thdPercent = 50;
  general.settings.voltageIrrAllowed = 0;
  general.settings.freqIrrAllowed = 0;
  general.settings.currentZeroAdc = 2040;
  general.settings.voltageZeroAdc = 2040;
}