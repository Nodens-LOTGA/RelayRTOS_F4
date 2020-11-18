#include "setupscreen.h"
#include "FreeRTOSConfig.h"
#include "colors.h"
#include "settings.h"

Screen setupScreen;

// ID: 0
uint16_t *setupItemParams[] = {&(general.screens.setupScrValues.itemNumber)};
Label setupItemLabel = CREATE_LABEL_P(2, 0, L"Пункт %d", 2, 0, &arialBlack_10ptFontInfo, &blackColor, setupItemParams);

// ID: 1
Label setupSetupLabel = CREATE_LABEL(2, 20, L"УСТАНОВИТЕ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 2
Label setupVoltage1Label = CREATE_LABEL(2, 40, L"МАКСИМАЛЬНО", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 3
Label setupVoltage2Label = CREATE_LABEL(2, 40, L"МИНИМАЛЬНО", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 4
Label setupVoltage3Label = CREATE_LABEL(2, 60, L"ДОПУСТИМОЕ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 5
Label setupVoltage4Label = CREATE_LABEL(2, 80, L"НАПРЯЖЕНИЕ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 6
uint16_t *setupVoltageNParams[] = {&(general.screens.setupScrValues.voltage)};
Label setupVoltageNLabel = CREATE_LABEL_P(37, 101, L"%d V", 1, 0, &arialBlack_24ptFontInfo, &redColor, setupVoltageNParams);

// ID: 7
Label setupCurrent1Label = CREATE_LABEL(2, 40, L"МАКСИМАЛЬНО", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 8
Label setupCurrent2Label = CREATE_LABEL(2, 60, L"ДОПУСТИМУЮ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 9
Label setupCurrent3Label = CREATE_LABEL(2, 80, L"СИЛУ ТОКА", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 10
uint16_t *setupCurrentNParams[] = {&(general.screens.setupScrValues.current)};
Label setupCurrentNLabel = CREATE_LABEL_P(47, 101, L"%d A", 1, 0, &arialBlack_24ptFontInfo, &redColor, setupCurrentNParams);

// ID: 11
Label setupKW1Label = CREATE_LABEL(2, 80, L"МОЩНОСТЬ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 12
uint16_t *setupKWNParams[] = {
    &(general.screens.setupScrValues.kWInteger),
    &(general.screens.setupScrValues.kWFract),
};
Label setupKWNLabel = CREATE_LABEL_P(25, 101, L"%d.%d kW", 1, 0, &arialBlack_24ptFontInfo, &redColor, setupKWNParams);

// ID: 13
Label setupLoad1Label = CREATE_LABEL(2, 20, L"ВЫБЕРИТЕ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 14
Label setupLoad2Label = CREATE_LABEL(2, 40, L"ПАРАМЕТР", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 15
Label setupLoad3Label = CREATE_LABEL(2, 60, L"НАГРУЗКИ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 16
Label setupLoadA1Label = CREATE_LABEL(11, 79, L"СИЛА ТОКА", 1, 0, &arialBlack_16ptFontInfo, &redColor);
// ID: 17
Label setupLoadA2Label = CREATE_LABEL(67, 101, L"(A)", 1, 0, &arialBlack_16ptFontInfo, &redColor);
// ID: 18
Label setupLoadkW1Label = CREATE_LABEL(12, 79, L"МОЩНОСТЬ", 1, 0, &arialBlack_16ptFontInfo, &redColor);
// ID: 19
Label setupLoadkW2Label = CREATE_LABEL(57, 101, L"(kW)", 1, 0, &arialBlack_16ptFontInfo, &redColor);

// ID: 20
Label setupTime1Label = CREATE_LABEL(2, 40, L"ВРЕМЯ И ДАТУ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 21
uint16_t *setupTimeHParams[] = {&(general.screens.setupScrValues.setupTimeHours)};
Label setupTimeHLabel = CREATE_LABEL_P_A(77, 72, L"%02d", 1, 0, &arialBlack_20ptFontInfo, &blueColor, setupTimeHParams, TextAlignRight);
// ID: 22
uint16_t *setupTimeMParams[] = {&(general.screens.setupScrValues.setupTimeMinutes)};
Label setupTimeMLabel = CREATE_LABEL_P(81, 72, L"%02d", 1, 0, &arialBlack_20ptFontInfo, &blueColor, setupTimeMParams);
// ID: 23
Label setupTimeSepLabel = CREATE_LABEL_A(81, 76, L":", 1, 0, &arialBlack_12ptFontInfo, &blueColor, TextAlignRight);

// ID: 24
uint16_t *setupDateDParams[] = {&(general.screens.setupScrValues.setupDateDay)};
Label setupDateDLabel = CREATE_LABEL_P(1, 98, L"%02d", 1, 0, &arialBlack_20ptFontInfo, &blueColor, setupDateDParams);
// ID: 25
uint16_t *setupDateMParams[] = {&(general.screens.setupScrValues.setupDateMonth)};
Label setupDateMLabel = CREATE_LABEL_P(40, 98, L"%02d", 1, 0, &arialBlack_20ptFontInfo, &blueColor, setupDateMParams);
// ID: 26
uint16_t *setupDateYParams[] = {&(general.screens.setupScrValues.setupDateYear)};
Label setupDateYLabel = CREATE_LABEL_P(78, 98, L"%04d", 1, 0, &arialBlack_20ptFontInfo, &blueColor, setupDateYParams);
// ID: 27
Label setupDateDot1Label = CREATE_LABEL(35, 107, L".", 1, 0, &arialBlack_12ptFontInfo, &blueColor);
// ID: 28
Label setupDateDot2Label = CREATE_LABEL(74, 107, L".", 1, 0, &arialBlack_12ptFontInfo, &blueColor);

// ID: 29
Label setupLight1Label = CREATE_LABEL(2, 20, L"УСТАНОВИТЕ ВРЕМЯ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 30
Label setupLight2Label = CREATE_LABEL(2, 35, L"НАЧАЛА ПОДСВЕТКИ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 31
Label setupLight3Label = CREATE_LABEL(2, 50, L"ЭКРАНА", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 32
uint16_t *setupLightTimeParams[] = {&(general.screens.setupScrValues.lightTimeHours), &(general.screens.setupScrValues.lightTimeMinutes)};
Label setupLightTimeLabel = CREATE_LABEL_P(42, 101, L"%02d:%02d", 1, 0, &arialBlack_24ptFontInfo, &redColor, setupLightTimeParams);

// ID: 33
Label setupLightDur1Label = CREATE_LABEL(2, 35, L"ДЛИТЕЛЬНОСТЬ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 34
Label setupLightDur2Label = CREATE_LABEL(2, 50, L"ПОДСВЕТКИ ЭКРАНА", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 35
uint16_t *setupLightDurEnParams[] = {&(general.screens.setupScrValues.lightEnDuration)};
Label setupLightDurEnLabel = CREATE_LABEL_P_A(70, 101, L"%dч.", 1, 0, &arialBlack_24ptFontInfo, &redColor, setupLightDurEnParams, TextAlignRight);
// ID: 36
uint16_t *setupLightDurDisParams[] = {&(general.screens.setupScrValues.lightDisDuration)};
Label setupLightDurDisLabel = CREATE_LABEL_P(92, 101, L"%dч.", 1, 0, &arialBlack_24ptFontInfo, &blackColor, setupLightDurDisParams);
// ID: 37
Label setupLightDurSplitLabel = CREATE_LABEL(73, 101, L"I", 1, 25, &arialBlack_24ptFontInfo, &blueColor);

// ID: 38
Label setupBg1Label = CREATE_LABEL(2, 40, L"ВЫБЕРИТЕ ФОН", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 39
Label setupBg2Label = CREATE_LABEL(2, 60, L"ПОДСВЕТКИ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 40
Label setupBg3Label = CREATE_LABEL(2, 80, L"ЭКРАНА", 1, 0, &arialBlack_10ptFontInfo, &blueColor);

// ID: 41
Label setupOutputLabel = CREATE_LABEL(2, 20, L"ВЫХОД", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 42
Label setupAllowLabel = CREATE_LABEL(3, 96, L"ЗАПРЕЩЕН", 1, 0, &arialBlack_20ptFontInfo, &redColor);
// ID: 43
Label setupDenyLabel = CREATE_LABEL(5, 96, L"РАЗРЕШЕН", 1, 0, &arialBlack_20ptFontInfo, &redColor);

// ID: 44
Label setupTHD1Label = CREATE_LABEL(2, 40, L"ИСКАЖЕННОЙ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 45
Label setupTHD2Label = CREATE_LABEL(2, 60, L"ФОРМЫ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 46
Label setupTHD3Label = CREATE_LABEL(2, 80, L"НАПРЯЖЕНИЯ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);

// ID: 47
Label setupFreq1Label = CREATE_LABEL(2, 40, L"НЕСТАНДАРТНОЙ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 48
Label setupFreq2Label = CREATE_LABEL(2, 60, L"ЧАСТОТЫ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);

// ID: 49
Label setupUa1Label = CREATE_LABEL(2, 40, L"НЕДОПУСТИМОЙ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 50
Label setupUa2Label = CREATE_LABEL(2, 60, L"АМПЛИТУДЫ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);

// ID: 51
Label setupTHDP1Label = CREATE_LABEL(2, 20, L"УСТАНОВИТЕ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 52
Label setupTHDP2Label = CREATE_LABEL(2, 40, L"ДОПУСТИМЫЙ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 53
Label setupTHDP3Label = CREATE_LABEL(2, 60, L"% THD", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 54
Label setupTHDP4Label = CREATE_LABEL(20, 95, L"ЗАПРЕЩЕНО", 1, 0, &arialBlack_12ptFontInfo, &redColor);
// ID: 55
uint16_t *setupTHDPNParams[] = {&(general.screens.setupScrValues.thdPercent)};
Label setupTHDPNLabel = CREATE_LABEL_P(55, 101, L"%d%%", 1, 0, &arialBlack_24ptFontInfo, &redColor, setupTHDPNParams);

// ID: 56
Label setupDelay1Label = CREATE_LABEL(2, 20, L"ЗАДЕРЖКА", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 57
Label setupDelay2Label = CREATE_LABEL(2, 40, L"ВКЛЮЧЕНИЯ", 1, 0, &arialBlack_10ptFontInfo, &blueColor);
// ID: 58
uint16_t *setupDelayNParams[] = {&(general.screens.setupScrValues.delaySeconds)};
Label setupDelayNLabel = CREATE_LABEL_P(55, 101, L"%d с", 1, 0, &arialBlack_24ptFontInfo, &redColor, setupDelayNParams);

// ID: 59
Label setupTHDP5Label = CREATE_LABEL(7, 79, L"НАСТРОЙКА", 1, 0, &arialBlack_16ptFontInfo, &redColor);
// ID: 60
Label setupTHDP6Label = CREATE_LABEL(6, 101, L"ЗАПРЕЩЕНА", 1, 0, &arialBlack_16ptFontInfo, &redColor);
// ID: 61
Label setupDateDot3Label = CREATE_LABEL(147, 107, L"г.", 1, 0, &arialBlack_12ptFontInfo, &blueColor);
Label *setupScrLabels[] = {
    &setupItemLabel,        &setupSetupLabel,         &setupVoltage1Label,  &setupVoltage2Label,  &setupVoltage3Label,  &setupVoltage4Label,
    &setupVoltageNLabel,    &setupCurrent1Label,      &setupCurrent2Label,  &setupCurrent3Label,  &setupCurrentNLabel,  &setupKW1Label,
    &setupKWNLabel,         &setupLoad1Label,         &setupLoad2Label,     &setupLoad3Label,     &setupLoadA1Label,    &setupLoadA2Label,
    &setupLoadkW1Label,     &setupLoadkW2Label,       &setupTime1Label,     &setupTimeHLabel,     &setupTimeMLabel,     &setupTimeSepLabel,
    &setupDateDLabel,       &setupDateMLabel,         &setupDateYLabel,     &setupDateDot1Label,  &setupDateDot2Label,  &setupLight1Label,
    &setupLight2Label,      &setupLight3Label,        &setupLightTimeLabel, &setupLightDur1Label, &setupLightDur2Label, &setupLightDurEnLabel,
    &setupLightDurDisLabel, &setupLightDurSplitLabel, &setupBg1Label,       &setupBg2Label,       &setupBg3Label,       &setupOutputLabel,
    &setupAllowLabel,       &setupDenyLabel,          &setupTHD1Label,      &setupTHD2Label,      &setupTHD3Label,      &setupFreq1Label,
    &setupFreq2Label,       &setupUa1Label,           &setupUa2Label,       &setupTHDP1Label,     &setupTHDP2Label,     &setupTHDP3Label,
    &setupTHDP4Label,       &setupTHDPNLabel,         &setupDelay1Label,    &setupDelay2Label,    &setupDelayNLabel,    &setupTHDP5Label,
    &setupTHDP6Label,       &setupDateDot3Label};

int setupScrStrCount = sizeof(setupScrLabels) / sizeof(setupScrLabels[0]);

void InitSetupScreen(void) {
  setupScreen.labels = (Label **)setupScrLabels;
  setupScreen.strCount = setupScrStrCount;
}

void EnableSetupScrSet(void) {
  setupScreen.bgColor = bgColors[general.settings.bgColorType];
  general.screens.lastState = -1;
  general.screens.currentScreenState = 0;
  general.screens.lastScreen = general.screens.currentScreen;
  DisableStrings(&setupScreen, 0, setupScrStrCount);
  ShowScreen(&setupScreen, 1);
}

void UpdateSetupScrSet(uint32_t tickCount) {
  if (general.control.idleTime >= 15000) {
    general.screens.currentScreen = 0;
    general.screens.currentScreenState = 0;
    return;
  }
  static uint8_t lastIndex = 0;
  static uint8_t lastBgColorType = 0;
  setup_scr_set setNum = general.screens.currentScreenState;
  uint8_t lastVisible = 0;
  uint8_t currVisible = (tickCount / 500) % 2;
  if (lastIndex != setNum)
    DisableStrings(&setupScreen, 0, setupScrStrCount);

  lastVisible = GetStringVisible(&setupScreen, 0);
  if (lastVisible == 1 && currVisible == 0 && lastIndex == setNum) {
    ClearScreenValue(&setupScreen, 0);
  }
  SetStringVisible(&setupScreen, 0, currVisible != 0 ? 1 : 0);
  general.screens.setupScrValues.itemNumber = GetSetIndex(general.screens.currentScreenState);

  switch (setNum) {
  case MAX_VOLTAGE: {
    general.screens.setupScrValues.voltage = general.settings.maxVoltage;
    SetStringVisible(&setupScreen, 1, 1);
    SetStringVisible(&setupScreen, 2, 1);
    SetStringVisible(&setupScreen, 4, 1);
    SetStringVisible(&setupScreen, 5, 1);
    SetStringVisible(&setupScreen, 6, 1);
    break;
  }
  case MIN_VOLTAGE: {
    general.screens.setupScrValues.voltage = general.settings.minVoltage;
    SetStringVisible(&setupScreen, 1, 1);
    SetStringVisible(&setupScreen, 3, 1);
    SetStringVisible(&setupScreen, 4, 1);
    SetStringVisible(&setupScreen, 5, 1);
    SetStringVisible(&setupScreen, 6, 1);
    break;
  }
  case MAX_CURRENT: {
    general.screens.setupScrValues.current = general.settings.maxCurrent;
    SetStringVisible(&setupScreen, 1, 1);
    SetStringVisible(&setupScreen, 7, 1);
    SetStringVisible(&setupScreen, 8, 1);
    SetStringVisible(&setupScreen, 9, 1);
    SetStringVisible(&setupScreen, 10, 1);
    break;
  }
  case MAX_KW: {
    general.screens.setupScrValues.kWInteger = (general.settings.maxCurrent * 230 + 50) / 100 / 10;
    general.screens.setupScrValues.kWFract = (general.settings.maxCurrent * 230 + 50) / 100 % 10;
    SetStringVisible(&setupScreen, 1, 1);
    SetStringVisible(&setupScreen, 7, 1);
    SetStringVisible(&setupScreen, 8, 1);
    SetStringVisible(&setupScreen, 11, 1);
    SetStringVisible(&setupScreen, 12, 1);
    break;
  }
  case ANY_FREQ: {
    SetStringVisible(&setupScreen, 41, 1);
    SetStringVisible(&setupScreen, 47, 1);
    SetStringVisible(&setupScreen, 48, 1);
    if (general.settings.freqIrrAllowed == 0) {
      ClearScreenValue(&setupScreen, 43);
      SetStringVisible(&setupScreen, 42, 1);
      SetStringVisible(&setupScreen, 43, 0);
    }
    if (general.settings.freqIrrAllowed == 1) {
      ClearScreenValue(&setupScreen, 42);
      SetStringVisible(&setupScreen, 43, 1);
      SetStringVisible(&setupScreen, 42, 0);
    }
    break;
  }
  case ANY_VOLTAGE: {
    SetStringVisible(&setupScreen, 41, 1);
    SetStringVisible(&setupScreen, 46, 1);
    SetStringVisible(&setupScreen, 49, 1);
    SetStringVisible(&setupScreen, 50, 1);
    if (general.settings.voltageIrrAllowed == 0) {
      ClearScreenValue(&setupScreen, 43);
      SetStringVisible(&setupScreen, 42, 1);
      SetStringVisible(&setupScreen, 43, 0);
    }
    if (general.settings.voltageIrrAllowed == 1) {
      ClearScreenValue(&setupScreen, 42);
      SetStringVisible(&setupScreen, 43, 1);
      SetStringVisible(&setupScreen, 42, 0);
    }
    break;
  }
  case IRREG_SIN: {
    SetStringVisible(&setupScreen, 41, 1);
    SetStringVisible(&setupScreen, 44, 1);
    SetStringVisible(&setupScreen, 45, 1);
    SetStringVisible(&setupScreen, 46, 1);
    if (general.settings.thdAllowed == 0) {
      ClearScreenValue(&setupScreen, 43);
      SetStringVisible(&setupScreen, 42, 1);
      SetStringVisible(&setupScreen, 43, 0);
    }
    if (general.settings.thdAllowed == 1) {
      ClearScreenValue(&setupScreen, 42);
      SetStringVisible(&setupScreen, 43, 1);
      SetStringVisible(&setupScreen, 42, 0);
    }
    break;
  }
  case PERCENT_IRREG_SIN: {
    general.screens.setupScrValues.thdPercent = general.settings.thdPercent;
    SetStringVisible(&setupScreen, 51, 1);
    SetStringVisible(&setupScreen, 52, 1);
    SetStringVisible(&setupScreen, 53, 1);
    SetStringVisible(&setupScreen, 55, general.settings.thdAllowed ? 0 : 1);
    SetStringVisible(&setupScreen, 59, general.settings.thdAllowed);
    SetStringVisible(&setupScreen, 60, general.settings.thdAllowed);
    break;
  }
  case DELAY_START: {
    general.screens.setupScrValues.delaySeconds = general.settings.startDelayS;
    SetStringVisible(&setupScreen, 56, 1);
    SetStringVisible(&setupScreen, 57, 1);
    SetStringVisible(&setupScreen, 58, 1);
    break;
  }
  case TYPE_LOAD: {
    SetStringVisible(&setupScreen, 13, 1);
    SetStringVisible(&setupScreen, 14, 1);
    SetStringVisible(&setupScreen, 15, 1);
    if (general.settings.loadType == 0) {
      ClearScreenValue(&setupScreen, 18);
      ClearScreenValue(&setupScreen, 19);
      SetStringVisible(&setupScreen, 16, 1);
      SetStringVisible(&setupScreen, 17, 1);
      SetStringVisible(&setupScreen, 18, 0);
      SetStringVisible(&setupScreen, 19, 0);
    }
    if (general.settings.loadType == 1) {
      ClearScreenValue(&setupScreen, 16);
      ClearScreenValue(&setupScreen, 17);
      SetStringVisible(&setupScreen, 16, 0);
      SetStringVisible(&setupScreen, 17, 0);
      SetStringVisible(&setupScreen, 18, 1);
      SetStringVisible(&setupScreen, 19, 1);
    }
    break;
  }
  case SET_TIME: {
    general.screens.setupScrValues.setupTimeHours = general.settings.currentDate.hours;
    general.screens.setupScrValues.setupTimeMinutes = general.settings.currentDate.minutes;

    general.screens.setupScrValues.setupDateDay = general.settings.currentDate.day;
    general.screens.setupScrValues.setupDateMonth = general.settings.currentDate.month;
    general.screens.setupScrValues.setupDateYear = general.settings.currentDate.year + 2000;

    setupTimeHLabel.color = (general.screens.setupScrValues.selectedEl == 0) ? &redColor : &blueColor;
    setupTimeMLabel.color = (general.screens.setupScrValues.selectedEl == 1) ? &redColor : &blueColor;
    setupDateDLabel.color = (general.screens.setupScrValues.selectedEl == 2) ? &redColor : &blueColor;
    setupDateMLabel.color = (general.screens.setupScrValues.selectedEl == 3) ? &redColor : &blueColor;
    setupDateYLabel.color = (general.screens.setupScrValues.selectedEl == 4) ? &redColor : &blueColor;

    SetStringVisible(&setupScreen, 1, 1);
    SetStringVisible(&setupScreen, 20, 1);

    SetStringVisible(&setupScreen, 21, 1);
    SetStringVisible(&setupScreen, 22, 1);
    SetStringVisible(&setupScreen, 23, 1);
    SetStringVisible(&setupScreen, 24, 1);
    SetStringVisible(&setupScreen, 25, 1);
    SetStringVisible(&setupScreen, 26, 1);
    SetStringVisible(&setupScreen, 27, 1);
    SetStringVisible(&setupScreen, 28, 1);
    SetStringVisible(&setupScreen, 61, 1);
    break;
  }
  case START_LIGHT_TIME: {
    general.screens.setupScrValues.lightTimeHours = general.settings.lightTimeH;
    general.screens.setupScrValues.lightTimeMinutes = general.settings.lightTimeM;
    SetStringVisible(&setupScreen, 29, 1);
    SetStringVisible(&setupScreen, 30, 1);
    SetStringVisible(&setupScreen, 31, 1);
    SetStringVisible(&setupScreen, 32, 1);
    break;
  }
  case DURATION_LIGHT: {
    general.screens.setupScrValues.lightEnDuration = general.settings.lightDurH;
    general.screens.setupScrValues.lightDisDuration = 24 - general.settings.lightDurH;
    SetStringVisible(&setupScreen, 1, 1);
    SetStringVisible(&setupScreen, 33, 1);
    SetStringVisible(&setupScreen, 34, 1);
    SetStringVisible(&setupScreen, 35, 1);
    SetStringVisible(&setupScreen, 36, 1);
    SetStringVisible(&setupScreen, 37, 1);
    break;
  }
  case COLOR_BG: {
    SetStringVisible(&setupScreen, 38, 1);
    SetStringVisible(&setupScreen, 39, 1);
    SetStringVisible(&setupScreen, 40, 1);
    if (lastBgColorType != general.settings.bgColorType) {
      lastBgColorType = general.settings.bgColorType;
      setupScreen.bgColor = bgColors[lastBgColorType];
      ShowScreen(&setupScreen, 1);
      return;
    }
    break;
  }
  default:
    return;
  }
  ShowScreen(&setupScreen, lastIndex != setNum);
  lastIndex = setNum;
}

uint8_t GetSetIndex(uint8_t currentSet) {
  switch (currentSet) {
  case MAX_VOLTAGE:
    return 1;
  case MIN_VOLTAGE:
    return 2;
  case MAX_CURRENT:
  case MAX_KW:
    return 3;
  case ANY_FREQ:
    return 4;
  case ANY_VOLTAGE:
    return 5;
  case IRREG_SIN:
    return 6;
  case PERCENT_IRREG_SIN:
    return 7;
  case DELAY_START:
    return 8;
  case TYPE_LOAD:
    return 9;
  case SET_TIME:
    return 10;
  case START_LIGHT_TIME:
    return 11;
  case DURATION_LIGHT:
    return 12;
  case COLOR_BG:
    return 13;
  default:
    return 0;
  }
}