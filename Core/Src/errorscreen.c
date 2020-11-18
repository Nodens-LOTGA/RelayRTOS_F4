#include "colors.h"
#include "screens.h"
#include "settings.h"

Screen errorsScreen;

uint16_t *errorsDateBeginParams[] = {
    &(general.screens.errorsScrValues.currentError.fromDate.hours), &(general.screens.errorsScrValues.currentError.fromDate.minutes),
    &(general.screens.errorsScrValues.currentError.fromDate.day), &(general.screens.errorsScrValues.currentError.fromDate.month),
    &(general.screens.errorsScrValues.currentError.fromDate.year)};
Label errorsL0Date = CREATE_LABEL_P(4, 64, L"C %02d:%02d/%02d.%02d.%02dг.", 1, 0, &arialBlack_10ptFontInfo, &blueColor, errorsDateBeginParams);
uint16_t *errorDateEndParams[] = {&(general.screens.errorsScrValues.currentError.toDate.hours), &(general.screens.errorsScrValues.currentError.toDate.minutes),
                                  &(general.screens.errorsScrValues.currentError.toDate.day), &(general.screens.errorsScrValues.currentError.toDate.month),
                                  &(general.screens.errorsScrValues.currentError.toDate.year)};
Label errorsL1Date = CREATE_LABEL_P(4, 78, L"До %02d:%02d/%02d.%02d.%02dг.", 1, 0, &arialBlack_10ptFontInfo, &blueColor, errorDateEndParams);
Label errorsL2Manual = CREATE_LABEL(4, 5, L"Отключение", 1, 0, &arialBlack_12ptFontInfo, &redColor);
Label errorsL3Manual = CREATE_LABEL(4, 23, L"нагрзуки", 1, 0, &arialBlack_12ptFontInfo, &redColor);
Label errorsL4Manual = CREATE_LABEL(4, 42, L"пользователем", 1, 0, &arialBlack_12ptFontInfo, &redColor);
Label errorsL5Power = CREATE_LABEL(4, 23, L"электроэнергии", 1, 0, &arialBlack_12ptFontInfo, &redColor);
uint16_t *errorVoltageParams[] = {&(general.screens.errorsScrValues.currentError.value)};
Label errorsL6Voltage = CREATE_LABEL_P(4, 5, L"На входе %d V", 1, 0, &arialBlack_12ptFontInfo, &redColor, errorVoltageParams);
uint16_t *errorCurrentParams[] = {&(general.screens.errorsScrValues.currentError.value)};
Label errorsL7Current = CREATE_LABEL_P(4, 5, L"%d А", 1, 0, &arialBlack_12ptFontInfo, &redColor, errorCurrentParams);
uint16_t *errorkWParams[] = {&(general.screens.errorsScrValues.currentError.value)};
Label errorsL8kW = CREATE_LABEL_P(4, 5, L"%d kW", 1, 0, &arialBlack_12ptFontInfo, &redColor, errorkWParams);
uint16_t *errorUaParams[] = {&(general.screens.errorsScrValues.currentError.value)};
Label errorsL9Ua = CREATE_LABEL_P(4, 5, L"Ua = %d kV", 1, 0, &arialBlack_12ptFontInfo, &redColor, errorUaParams);
uint16_t *errorDSParams[] = {&(general.screens.errorsScrValues.currentError.value)};
Label errorsL10DS = CREATE_LABEL_P(4, 5, L"%d%% THD", 1, 0, &arialBlack_12ptFontInfo, &redColor, errorDSParams);
uint16_t *errorFreqParams[] = {&(general.screens.errorsScrValues.currentError.value)};
Label errorsL11Freq = CREATE_LABEL_P(4, 5, L"%d Hz", 1, 0, &arialBlack_12ptFontInfo, &redColor, errorFreqParams);

Label *errorsScrLabels[] = {&errorsL0Date,    &errorsL1Date,    &errorsL2Manual, &errorsL3Manual, &errorsL4Manual, &errorsL5Power,
                            &errorsL6Voltage, &errorsL7Current, &errorsL8kW,     &errorsL9Ua,     &errorsL10DS,    &errorsL11Freq};

int errorsScrStrCount = sizeof(errorsScrLabels) / sizeof(errorsScrLabels[0]);

void InitErrorsScreen(void) {
  errorsScreen.labels = (Label **)errorsScrLabels;
  errorsScreen.strCount = errorsScrStrCount;
}

void EnableErrorScrSet(void) {
  errorsScreen.bgColor = bgColors[general.settings.bgColorType];
  if (general.screens.errorsScrValues.errorsOrder[0] == 0 || general.screens.errorsScrValues.errors[0].value == 0) {
    general.screens.currentScreen = 0;
    return;
  }
  general.screens.lastState = -1;
  general.screens.currentScreenState = 0;
  general.screens.lastScreen = general.screens.currentScreen;
  DisableStrings(&errorsScreen, 0, errorsScrStrCount);
  ShowScreen(&errorsScreen, 1);
}

void UpdateErrorScrSet(void) {
  uint8_t currentState = general.screens.currentScreenState;
  if (currentState >= general.screens.errorsScrValues.errors[0].value)
    return;
  ErrorType setNum = general.screens.errorsScrValues.errorsOrder[currentState];
  if (general.control.idleTime >= 15000) {
    general.screens.currentScreen = 0;
    general.screens.currentScreenState = 0;
    return;
  }
  if (general.screens.lastState == currentState)
    return;
  if (setNum == 0) {
    general.screens.currentScreen = 0;
    general.screens.currentScreenState = 0;
    return;
  }

  general.screens.lastState = currentState;
  DisableStrings(&errorsScreen, 0, errorsScrStrCount);
  general.screens.errorsScrValues.currentError.value = general.screens.errorsScrValues.errors[setNum].value;
  general.screens.errorsScrValues.currentError.fromDate.day = general.screens.errorsScrValues.errors[setNum].fromDate.day;
  general.screens.errorsScrValues.currentError.fromDate.month = general.screens.errorsScrValues.errors[setNum].fromDate.month;
  general.screens.errorsScrValues.currentError.fromDate.year = general.screens.errorsScrValues.errors[setNum].fromDate.year;
  general.screens.errorsScrValues.currentError.fromDate.hours = general.screens.errorsScrValues.errors[setNum].fromDate.hours;
  general.screens.errorsScrValues.currentError.fromDate.minutes = general.screens.errorsScrValues.errors[setNum].fromDate.minutes;
  general.screens.errorsScrValues.currentError.toDate.day = general.screens.errorsScrValues.errors[setNum].toDate.day;
  general.screens.errorsScrValues.currentError.toDate.month = general.screens.errorsScrValues.errors[setNum].toDate.month;
  general.screens.errorsScrValues.currentError.toDate.year = general.screens.errorsScrValues.errors[setNum].toDate.year;
  general.screens.errorsScrValues.currentError.toDate.hours = general.screens.errorsScrValues.errors[setNum].toDate.hours;
  general.screens.errorsScrValues.currentError.toDate.minutes = general.screens.errorsScrValues.errors[setNum].toDate.minutes;

  SetStringVisible(&errorsScreen, 0, 1);
  SetStringVisible(&errorsScreen, 1, 1);
  switch (setNum) {
  case ManualPowerOffError:
    SetStringVisible(&errorsScreen, 2, 1);
    SetStringVisible(&errorsScreen, 3, 1);
    SetStringVisible(&errorsScreen, 4, 1);
    break;
  case PowerOffError:
    SetStringVisible(&errorsScreen, 2, 1);
    SetStringVisible(&errorsScreen, 5, 1);
    break;
  case VoltageRmsError:
    SetStringVisible(&errorsScreen, 6, 1);
    break;
  case ThdError:
    SetStringVisible(&errorsScreen, 10, 1);
    break;
  case CurrentError:
    SetStringVisible(&errorsScreen, 7, 1);
    break;
  case VoltageAmpError:
    SetStringVisible(&errorsScreen, 9, 1);
    break;
  case FreqError:
    SetStringVisible(&errorsScreen, 11, 1);
    break;
  }
  ShowScreen(&errorsScreen, 1);
}