#include "colors.h"
#include "screens.h"
#include "settings.h"

Screen afterErrScreen;

uint16_t *afterErrorsDateBeginParams[] = {
    &(general.screens.crashesScrValues.currentCrash.date.hours), &(general.screens.crashesScrValues.currentCrash.date.minutes),
    &(general.screens.crashesScrValues.currentCrash.date.day), &(general.screens.crashesScrValues.currentCrash.date.month),
    &(general.screens.crashesScrValues.currentCrash.date.year)};
Label afterErrorsL0Date = CREATE_LABEL_P(4, 5, L"%02d:%02d/%02d.%02d.%02dг.", 1, 0, &arialBlack_10ptFontInfo, &blueColor, afterErrorsDateBeginParams);
uint16_t *afterErrorsTotalDaysParams[] = {&(general.eeTime.daysWork)};
Label afterErrorsL1TotalDays = CREATE_LABEL_P(4, 64, L"%d суток", 1, 0, &arialBlack_10ptFontInfo, &blueColor, afterErrorsTotalDaysParams);
Label afterErrorsL2TotalDays = CREATE_LABEL(4, 5, L"Общее время", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
Label afterErrorsL3TotalDays = CREATE_LABEL(4, 23, L"работы", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
Label afterErrorsL4TotalDays = CREATE_LABEL(4, 41, L"устройства", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
Label afterErrorsL5Warranty = CREATE_LABEL(4, 80, L"Гарантийные", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL6Warranty = CREATE_LABEL(4, 95, L"обязательства", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL7Warranty = CREATE_LABEL(4, 110, L"сняты", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL8Voltage = CREATE_LABEL(4, 20, L"На входе", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
Label afterErrorsL9Voltage = CREATE_LABEL(4, 35, L"зафиксировано", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
Label afterErrorsL10Voltage = CREATE_LABEL(4, 50, L"напряжение", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
uint16_t *afterErrorsVoltageParams[] = {&(general.screens.crashesScrValues.currentCrash.value)};
Label afterErrorsL11Voltage = CREATE_LABEL_P(4, 65, L"%d V", 1, 0, &arialBlack_10ptFontInfo, &blackColor, afterErrorsVoltageParams);
Label afterErrorsL12Short = CREATE_LABEL(4, 20, L"Зафиксировано", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
Label afterErrorsL13Short = CREATE_LABEL(4, 35, L"короткое", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
Label afterErrorsL14Short = CREATE_LABEL(4, 50, L"замыкание на", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
Label afterErrorsL15Short = CREATE_LABEL(4, 65, L"выходе", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
Label afterErrorsL16Temp = CREATE_LABEL(4, 20, L"Недопустимая t", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
Label afterErrorsL17Temp = CREATE_LABEL(4, 35, L"эксплуатации", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
uint16_t *afterErrorsTempParams[] = {&(general.screens.crashesScrValues.currentCrash.value)};
Label afterErrorsL18Temp = CREATE_LABEL_P(4, 50, L"устройства +%d*C", 1, 0, &arialBlack_10ptFontInfo, &blackColor, afterErrorsTempParams);
Label afterErrorsL19InTemp = CREATE_LABEL(4, 20, L"Перегрев", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL20InTemp = CREATE_LABEL(4, 35, L"клеммы вход", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL21OutTemp = CREATE_LABEL(4, 20, L"Перегрев", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL22OutTemp = CREATE_LABEL(4, 35, L"клеммы выход", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL23OpenInSens = CREATE_LABEL(4, 20, L"ОБРЫВ ДАТЧИКА", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL24OpenInSens = CREATE_LABEL(4, 35, L"ПЕРЕГРЕВА КЛЕММЫ", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL25OpenInSens = CREATE_LABEL(4, 50, L"ВХОД", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL26OpenOutSens = CREATE_LABEL(4, 20, L"ОБРЫВ ДАТЧИКА", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL27OpenOutSens = CREATE_LABEL(4, 35, L"ПЕРЕГРЕВА КЛЕММЫ", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL28OpenOutSens = CREATE_LABEL(4, 50, L"ВЫХОД", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL29Gap = CREATE_LABEL(4, 20, L"Пропуски при", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL30Gap = CREATE_LABEL(4, 35, L"срабатывании", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label afterErrorsL31Gap = CREATE_LABEL(4, 50, L"силового реле", 1, 0, &arialBlack_10ptFontInfo, &redColor);

Label *afterErrScrLabels[] = {&afterErrorsL0Date,         &afterErrorsL1TotalDays,   &afterErrorsL2TotalDays,    &afterErrorsL3TotalDays,
                              &afterErrorsL4TotalDays,    &afterErrorsL5Warranty,    &afterErrorsL6Warranty,     &afterErrorsL7Warranty,
                              &afterErrorsL8Voltage,      &afterErrorsL9Voltage,     &afterErrorsL10Voltage,     &afterErrorsL11Voltage,
                              &afterErrorsL12Short,       &afterErrorsL13Short,      &afterErrorsL14Short,       &afterErrorsL15Short,
                              &afterErrorsL16Temp,        &afterErrorsL17Temp,       &afterErrorsL18Temp,        &afterErrorsL19InTemp,
                              &afterErrorsL20InTemp,      &afterErrorsL21OutTemp,    &afterErrorsL22OutTemp,     &afterErrorsL23OpenInSens,
                              &afterErrorsL24OpenInSens,  &afterErrorsL25OpenInSens, &afterErrorsL26OpenOutSens, &afterErrorsL27OpenOutSens,
                              &afterErrorsL28OpenOutSens, &afterErrorsL29Gap,        &afterErrorsL30Gap,         &afterErrorsL31Gap};

int afterErrStrCount = sizeof(afterErrScrLabels) / sizeof(afterErrScrLabels[0]);

void InitAfterErrScreen(void) {
  afterErrScreen.labels = afterErrScrLabels;
  afterErrScreen.strCount = afterErrStrCount;
}

void EnableAfterErrorScrSet(void) {
  afterErrScreen.bgColor = bgColors[general.settings.bgColorType];
  general.screens.crashesScrValues.crashesOrder[0] = 0;
  general.screens.lastState = -1;
  general.screens.currentScreenState = 0;
  general.screens.lastScreen = general.screens.currentScreen;
  DisableStrings(&afterErrScreen, 0, afterErrStrCount);
  ShowScreen(&afterErrScreen, 1);
}

void UpdateAfterErrorScrSet(void) {
  uint8_t currentState = general.screens.currentScreenState;
  if (currentState > general.screens.crashesScrValues.crashes[0].value) // + экран со временем
    return;
  CrashType setNum = general.screens.crashesScrValues.crashesOrder[currentState];
  if (general.control.idleTime >= 15000) {
    general.screens.currentScreen = 0;
    general.screens.currentScreenState = 0;
    return;
  }
  if (general.screens.lastState == currentState)
    return;
  if (setNum == 0 && currentState != 0) {
    general.screens.currentScreen = 0;
    general.screens.currentScreenState = 0;
    return;
  }
  general.screens.lastState = currentState;
  DisableStrings(&afterErrScreen, 0, afterErrStrCount);
  general.screens.crashesScrValues.currentCrash.value = general.screens.crashesScrValues.crashes[setNum].value;
  general.screens.crashesScrValues.currentCrash.date.day = general.screens.crashesScrValues.crashes[setNum].date.day;
  general.screens.crashesScrValues.currentCrash.date.month = general.screens.crashesScrValues.crashes[setNum].date.month;
  general.screens.crashesScrValues.currentCrash.date.year = general.screens.crashesScrValues.crashes[setNum].date.year;
  general.screens.crashesScrValues.currentCrash.date.hours = general.screens.crashesScrValues.crashes[setNum].date.hours;
  general.screens.crashesScrValues.currentCrash.date.minutes = general.screens.crashesScrValues.crashes[setNum].date.minutes;
  SetStringVisible(&afterErrScreen, 0, 1);
  switch (setNum) {
  case OvervoltageCrash:
  case ShortCircuitCrash:
  case TempCrash:
    if (general.screens.crashesScrValues.currentCrash.value < 256) {
      general.screens.crashesScrValues.currentCrash.value = 256 - general.screens.crashesScrValues.currentCrash.value;
      afterErrorsL18Temp.str = L"устройства -%d*C";
    } else {
      general.screens.crashesScrValues.currentCrash.value = general.screens.crashesScrValues.currentCrash.value - 256;
      afterErrorsL18Temp.str = L"устройства +%d*C";
    }
    SetStringVisible(&afterErrScreen, 5, 1);
    SetStringVisible(&afterErrScreen, 6, 1);
    SetStringVisible(&afterErrScreen, 7, 1);
    break;
  default:
    break;
  }
  switch (setNum) {
  case OvervoltageCrash:
    SetStringVisible(&afterErrScreen, 8, 1);
    SetStringVisible(&afterErrScreen, 9, 1);
    SetStringVisible(&afterErrScreen, 10, 1);
    SetStringVisible(&afterErrScreen, 11, 1);
    break;
  case ShortCircuitCrash:
    SetStringVisible(&afterErrScreen, 12, 1);
    SetStringVisible(&afterErrScreen, 13, 1);
    SetStringVisible(&afterErrScreen, 14, 1);
    SetStringVisible(&afterErrScreen, 15, 1);
    break;
  case TempCrash:
    SetStringVisible(&afterErrScreen, 16, 1);
    SetStringVisible(&afterErrScreen, 17, 1);
    SetStringVisible(&afterErrScreen, 18, 1);
    break;
  case InHeatCrash:
    SetStringVisible(&afterErrScreen, 19, 1);
    SetStringVisible(&afterErrScreen, 20, 1);
    break;
  case OutHeatCrash:
    SetStringVisible(&afterErrScreen, 21, 1);
    SetStringVisible(&afterErrScreen, 22, 1);
    break;
  case InTempSensCrash:
    SetStringVisible(&afterErrScreen, 23, 1);
    SetStringVisible(&afterErrScreen, 24, 1);
    SetStringVisible(&afterErrScreen, 25, 1);
    break;
  case OutTempSensCrash:
    SetStringVisible(&afterErrScreen, 26, 1);
    SetStringVisible(&afterErrScreen, 27, 1);
    SetStringVisible(&afterErrScreen, 28, 1);
    break;
  case RelayCrash:
    SetStringVisible(&afterErrScreen, 29, 1);
    SetStringVisible(&afterErrScreen, 30, 1);
    SetStringVisible(&afterErrScreen, 31, 1);
    break;
  default:
    SetStringVisible(&afterErrScreen, 0, 0);
    SetStringVisible(&afterErrScreen, 1, 1);
    SetStringVisible(&afterErrScreen, 2, 1);
    SetStringVisible(&afterErrScreen, 3, 1);
    SetStringVisible(&afterErrScreen, 4, 1);
    break;
  }
  ShowScreen(&afterErrScreen, 1);
}