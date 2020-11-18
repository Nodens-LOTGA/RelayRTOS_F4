#include "colors.h"
#include "screens.h"
#include "settings.h"
#include "utils.h"

#define I_OR_KW general.settings.loadType ? 7 : 6
#define ERR_OVERLAPPED_COUNT 7

Screen mainScreen;
Label mainScrL0Up = CREATE_LABEL(14, 4, L"ПАРАМЕТРЫ СЕТИ", 1, 0, &arialBlack_10ptFontInfo, &blackColor);
Label mainScrL1Error = CREATE_LABEL(6, 4, L"ПРИЧИНА ОТКЛЮЧЕНИЯ", 1, 0, &arialBlack_8ptFontInfo, &redColor);
uint16_t *mainScrVoltageParams[] = {&(general.screens.mainScrValues.voltageRms)};
Label mainScrL2Voltage = CREATE_LABEL_P(2, 29, L"%dV", 1, 0, &arialBlack_24ptFontInfo, &greenColor, mainScrVoltageParams);
uint16_t *mainScrUaParams[] = {&(general.screens.mainScrValues.voltageAmp)};
Label mainScrL3Ua = CREATE_LABEL_P_A(156, 39, L"Ua=%dV", 1, 25, &bahnschrift_14ptFontInfo, &greenColor, mainScrUaParams, TextAlignRight);
uint16_t *mainScrFreqParams[] = {&(general.screens.mainScrValues.freqInteger), &(general.screens.mainScrValues.freqFract)};
Label mainScrL4Freq = CREATE_LABEL_P(0, 66, L"%d.%dHz", 1, 25, &arialBlack_16ptFontInfo, &blueColor, mainScrFreqParams);
uint16_t *mainScrTHDParams[] = {&(general.screens.mainScrValues.thd)};
Label mainScrL5THD = CREATE_LABEL_P_A(157, 72, L"%d%%THD", 1, 25, &bahnschrift_14ptFontInfo, &greenColor, mainScrTHDParams, TextAlignRight);
uint16_t *mainScrCurrentParams[] = {&(general.screens.mainScrValues.currentInteger), &(general.screens.mainScrValues.currentFract)};
Label mainScrL6Current = CREATE_LABEL_P(2, 103, L"%d.%dA", 1, 0, &arialBlack_24ptFontInfo, &greenColor, mainScrCurrentParams);
uint16_t *mainScrKWParams[] = {&(general.screens.mainScrValues.kWInteger), &(general.screens.mainScrValues.kWFract)};
Label mainScrL7KW = CREATE_LABEL_P(2, 103, L"%d.%dkW", 1, 0, &arialBlack_24ptFontInfo, &greenColor, mainScrKWParams);
uint16_t *mainScrTimerParams[] = {&(general.screens.mainScrValues.delayS)};
Label mainScrL8Timer = CREATE_LABEL_P_A(156, 110, L"%dс", 1, 25, &arialBlack_12ptFontInfo, &blueColor, mainScrTimerParams, TextAlignRight);
uint16_t *mainScrMTimerParams[] = {&(general.screens.mainScrValues.delayM), &(general.screens.mainScrValues.delayS)};
Label mainScrL9MTimer = CREATE_LABEL_P_A(156, 110, L"%02d:%02d", 1, 25, &arialBlack_12ptFontInfo, &blueColor, mainScrMTimerParams, TextAlignRight);
uint16_t *mainScrHTimerParams[] = {&(general.screens.mainScrValues.delayH), &(general.screens.mainScrValues.delayM), &(general.screens.mainScrValues.delayS)};
Label mainScrL10HTimer = CREATE_LABEL_P_A(156, 110, L"%02d:%02d:%02d", 1, 25, &arialBlack_12ptFontInfo, &blueColor, mainScrHTimerParams, TextAlignRight);
Label mainScrL11Pause = CREATE_LABEL(2, 102, L"ПАУЗА", 1, 0, &arialBlack_16ptFontInfo, &redColor);
Label mainScrL12Defective0 = CREATE_LABEL(2, 89, L"Устройство не", 1, 0, &arialBlack_12ptFontInfo, &redColor);
Label mainScrL13Defective1 = CREATE_LABEL(2, 108, L"исправно", 1, 0, &arialBlack_12ptFontInfo, &redColor);
Label mainScrL14Temp0 = CREATE_LABEL(2, 95, L"Перегрев клеммы", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label mainScrL15Temp1 = CREATE_LABEL(2, 111, L"вход", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label mainScrL16Temp2 = CREATE_LABEL(2, 111, L"выход", 1, 0, &arialBlack_10ptFontInfo, &redColor);
Label mainScrL17Temp3 = CREATE_LABEL_A(158, 111, L"ПАУЗА", 1, 0, &arialBlack_10ptFontInfo, &redColor, TextAlignRight);
Label mainScrL18Temp4 = CREATE_LABEL(2, 95, L"Недопустимая t", 1, 0, &arialBlack_10ptFontInfo, &redColor);
uint16_t *mainScrTempParams[] = {&(general.screens.mainScrValues.temp)};
Label mainScrL19Temp5 = CREATE_LABEL_P(2, 111, L"эксплуатации +%d*C", 1, 0, &arialBlack_10ptFontInfo, &redColor, mainScrTempParams);
Label mainScrL20Short = CREATE_LABEL(2, 89, L"Короткое", 1, 0, &arialBlack_12ptFontInfo, &redColor);
Label mainScrL21Short = CREATE_LABEL(2, 108, L"замыкание", 1, 0, &arialBlack_12ptFontInfo, &redColor);

Label *mainScrLabels[] = {&mainScrL0Up,          &mainScrL1Error,       &mainScrL2Voltage, &mainScrL3Ua,     &mainScrL4Freq,    &mainScrL5THD,
                          &mainScrL6Current,     &mainScrL7KW,          &mainScrL8Timer,   &mainScrL9MTimer, &mainScrL10HTimer, &mainScrL11Pause,
                          &mainScrL12Defective0, &mainScrL13Defective1, &mainScrL14Temp0,  &mainScrL15Temp1, &mainScrL16Temp2,  &mainScrL17Temp3,
                          &mainScrL18Temp4,      &mainScrL19Temp5,      &mainScrL20Short,  &mainScrL21Short};

uint8_t mainScrStrCount = sizeof(mainScrLabels) / sizeof(mainScrLabels[0]);

void InitMainScreen(void) {
  mainScreen.labels = mainScrLabels;
  mainScreen.strCount = mainScrStrCount;
}

void UpdateMainScrValues(void) {
  //Изменение числовых значений главного экрана
  general.screens.mainScrValues.voltageRms = general.sensors.voltage;
  general.screens.mainScrValues.voltageAmp = general.sensors.voltageAmp;
  general.screens.mainScrValues.currentInteger = general.sensors.current / 10;
  general.screens.mainScrValues.currentFract = general.sensors.current % 10;
  general.screens.mainScrValues.kWInteger = ((uint64_t)general.sensors.voltage * general.sensors.current) / 1000 / 10;
  general.screens.mainScrValues.kWFract = ((uint64_t)general.sensors.voltage * general.sensors.current) / 1000 % 10;
  general.screens.mainScrValues.freqInteger = general.sensors.frequency / 10;
  general.screens.mainScrValues.freqFract = general.sensors.frequency % 10;
  general.screens.mainScrValues.thd = (math_sqrt(general.sensors.harm) * 100) / general.sensors.sin;
}

void EnableMainScrSet(void) {
  mainScreen.bgColor = bgColors[general.settings.bgColorType];
  general.screens.lastState = 255;
  general.screens.currentScreenState = general.screens.mainScrValues.isPaused;
  general.screens.lastScreen = general.screens.currentScreen;
  DisableStrings(&mainScreen, 0, mainScrStrCount);
  UpdateMainScrValues();
  ShowScreen(&mainScreen, 1);
}

void UpdateMainScrSet(uint32_t tickCount) {
  static uint8_t showOrder[ERR_OVERLAPPED_COUNT];
  static uint8_t currentErrors[ERR_OVERLAPPED_COUNT];
  static uint8_t lastShowedErr = 0;
  static uint32_t prevTick = 0;
  static uint32_t elapsTick = 0;
  static uint8_t overlappedErrors = 0;

  // Смена цветов/надписей только при первом появлении или изменение в текущих ошибках
  if (general.screens.mainScrValues.errorChanged || general.screens.lastState == 255) {
    UpdateMainScrValues();

    // Цвета по умолчанию
    mainScrL2Voltage.color = &greenColor;
    mainScrL3Ua.color = &greenColor;
    mainScrL4Freq.color = &blueColor;
    mainScrL5THD.color = &blueColor;
    mainScrL6Current.color = &greenColor;
    mainScrL7KW.color = &greenColor;
    mainScrL3Ua.intParams = mainScrUaParams;

    ShowString(&mainScreen, 2, 1);
    ShowString(&mainScreen, 3, 1);
    ShowString(&mainScreen, 4, 1);
    if (!general.settings.thdAllowed && !general.screens.mainScrValues.errors[ThdError]) {
      mainScrL5THD.str = L"THD";
      mainScrL5THD.strLength = 3;
      mainScrL5THD.intParamsCount = 0;
    } else {
      mainScrL5THD.str = L"%d%%THD";
      mainScrL5THD.strLength = 0;
      mainScrL5THD.intParamsCount = 1;
    }
    ShowString(&mainScreen, 5, 1);
    ShowString(&mainScreen, I_OR_KW, 1);
    ShowString(&mainScreen, 11, 0);
    ShowString(&mainScreen, 12, 0);
    ShowString(&mainScreen, 13, 0);
    ShowString(&mainScreen, 14, 0);
    ShowString(&mainScreen, 15, 0);
    ShowString(&mainScreen, 16, 0);
    ShowString(&mainScreen, 17, 0);
    ShowString(&mainScreen, 18, 0);
    ShowString(&mainScreen, 19, 0);
    ShowString(&mainScreen, 20, 0);
    ShowString(&mainScreen, 21, 0);

    // Проверка ошибок ErrorType и смена текста на красный
    if (general.screens.mainScrValues.errors[0]) {
      if (general.screens.mainScrValues.errors[VoltageRmsError])
        mainScrL2Voltage.color = &redColor;
      if (general.screens.mainScrValues.errors[VoltageAmpError] && !general.settings.voltageIrrAllowed) {
        mainScrL3Ua.color = &redColor;
        mainScrL3Ua.intParams = (uint16_t **)&general.screens.mainScrValues.errors[VoltageAmpError];
      }
      if (general.screens.mainScrValues.errors[FreqError] && !general.settings.freqIrrAllowed)
        mainScrL4Freq.color = &redColor;
      if (general.screens.mainScrValues.errors[ThdError])
        mainScrL5THD.color = &redColor;
      if (general.screens.mainScrValues.errors[CurrentError]) {
        mainScrL6Current.color = &redColor;
        mainScrL7KW.color = &redColor;
      }
    }
    // Контролируются ли значения?
    if (general.settings.voltageIrrAllowed)
      mainScrL3Ua.color = &purpleColor;
    if (general.settings.freqIrrAllowed)
      mainScrL4Freq.color = &purpleColor;

    // Общие для ошибок состояния
    if ((general.screens.mainScrValues.crashes[0] || general.screens.mainScrValues.errors[ManualPowerOffError]) &&
        general.screens.mainScrValues.errors[CurrentError] == 0 && general.control.relayState == 0) // Если ошибки, занимающие место силы тока
      ShowString(&mainScreen, I_OR_KW, 0);

    if (general.control.relayState == 0 && (general.screens.mainScrValues.errors[0] || general.screens.mainScrValues.crashes[0])) {
      ShowString(&mainScreen, 0, 0);
      ShowString(&mainScreen, 1, 1);
    } else {
      ShowString(&mainScreen, 0, 1);
      ShowString(&mainScreen, 1, 0);
    }

    currentErrors[0] = general.screens.mainScrValues.crashes[InTempSensCrash] || general.screens.mainScrValues.crashes[OutTempSensCrash] ||
                       general.screens.mainScrValues.crashes[RelayCrash];
    currentErrors[1] = general.screens.mainScrValues.crashes[InHeatCrash] ? 1 : 0;
    currentErrors[2] = general.screens.mainScrValues.crashes[OutHeatCrash] ? 1 : 0;
    currentErrors[3] = general.screens.mainScrValues.crashes[TempCrash] ? 1 : 0;
    currentErrors[4] = general.screens.mainScrValues.errors[CurrentError] ? 1 : 0;
    currentErrors[5] = general.screens.mainScrValues.errors[ManualPowerOffError] ? 1 : 0;
    currentErrors[6] = general.screens.mainScrValues.crashes[ShortCircuitCrash] ? 1 : 0;

    if (currentErrors[0] || currentErrors[1] || currentErrors[2] || currentErrors[3] || currentErrors[4] || currentErrors[5] || currentErrors[6])
      overlappedErrors = 1;
    else
      overlappedErrors = 0;

    general.screens.mainScrValues.errorChanged = 0;
  }

  // Обновление значений экрана (не самого экрана)
  elapsTick += tickCount - prevTick;
  prevTick = tickCount;
  if (elapsTick > 1000) {
    UpdateMainScrValues();
    elapsTick -= 1000;
  }

  // Таймер
  if (general.state.enableDelayMs != 0 && (general.state.enableDelayFlags & ENABLE_RELAY_DELAY == ENABLE_RELAY_DELAY)) {
    general.screens.mainScrValues.delayH = general.state.enableDelayMs / 1000 / 60 / 60;
    general.screens.mainScrValues.delayM = (general.state.enableDelayMs / 1000 / 60) % 60;
    general.screens.mainScrValues.delayS = (general.state.enableDelayMs / 1000) % 60;
    if (general.screens.mainScrValues.delayH != 0) {
      ShowString(&mainScreen, 8, 0);
      ShowString(&mainScreen, 9, 0);
      ShowString(&mainScreen, 10, 1);
    } else if (general.screens.mainScrValues.delayM != 0) {
      ShowString(&mainScreen, 8, 0);
      ShowString(&mainScreen, 9, 1);
      ShowString(&mainScreen, 10, 0);
    } else {
      ShowString(&mainScreen, 8, 1);
      ShowString(&mainScreen, 9, 0);
      ShowString(&mainScreen, 10, 0);
    }
  } else {
    ShowString(&mainScreen, 8, 0);
    ShowString(&mainScreen, 9, 0);
    ShowString(&mainScreen, 10, 0);
  }

  // Мигание красных надписей
  uint8_t show = (tickCount / 1000) % 2 == 0 ? 0 : 1;
  static uint8_t lastShow = 0;
  static uint8_t lastShowedAftErr = 0;
  if (lastShow != show) { // Чтобы не обновлять при том же состояние
    if ((general.screens.mainScrValues.errors[0] || general.screens.mainScrValues.crashes[0]) && general.control.relayState == 0) {
      if (show) { // TODO: отображение по кругу. Сейчас временное решение, пока не придумаю что-нибудь лучше
        if (overlappedErrors) {
          showOrder[lastShowedErr] = !show;
          while (currentErrors[(lastShowedErr + 1) % ERR_OVERLAPPED_COUNT] == 0)
            lastShowedErr++;
          lastShowedErr = (lastShowedErr + 1) % ERR_OVERLAPPED_COUNT;
        }
      }
      showOrder[lastShowedErr] = show;
      if (general.control.relayState == 0) {
        ShowString(&mainScreen, 1, show);
        ShowString(&mainScreen, 0, 0);
      }
      if (general.screens.mainScrValues.errors[0]) {
        if (general.screens.mainScrValues.errors[VoltageRmsError])
          ShowString(&mainScreen, 2, show);
        if (general.screens.mainScrValues.errors[VoltageAmpError] && !general.settings.voltageIrrAllowed)
          ShowString(&mainScreen, 3, show);
        if (general.screens.mainScrValues.errors[FreqError] && !general.settings.freqIrrAllowed)
          ShowString(&mainScreen, 4, show);
        if (general.screens.mainScrValues.errors[ThdError])
          ShowString(&mainScreen, 5, show);
        if (general.screens.mainScrValues.errors[CurrentError])
          ShowString(&mainScreen, I_OR_KW, showOrder[4]);
        if (general.screens.mainScrValues.errors[ManualPowerOffError])
          ShowString(&mainScreen, 11, showOrder[5]);
      }
      if (general.screens.mainScrValues.crashes[0]) {
        if (general.screens.mainScrValues.crashes[InTempSensCrash] || general.screens.mainScrValues.crashes[OutTempSensCrash] ||
            general.screens.mainScrValues.crashes[RelayCrash]) {
          ShowString(&mainScreen, 12, showOrder[0]);
          ShowString(&mainScreen, 13, showOrder[0]);
        }
        if (general.screens.mainScrValues.crashes[InHeatCrash] || general.screens.mainScrValues.crashes[OutHeatCrash]) {
          if (showOrder[1] || showOrder[2]) {
            ShowString(&mainScreen, 14, 1);
            ShowString(&mainScreen, 17, 1);
          } else {
            ShowString(&mainScreen, 14, 0);
            ShowString(&mainScreen, 17, 0);
          }
        }
        if (general.screens.mainScrValues.crashes[InHeatCrash]) {
          ShowString(&mainScreen, 15, showOrder[1]);
        }
        if (general.screens.mainScrValues.crashes[OutHeatCrash]) {
          ShowString(&mainScreen, 16, showOrder[2]);
        }
        if (general.screens.mainScrValues.crashes[TempCrash]) {
          if (general.screens.mainScrValues.crashes[TempCrash] < 256) {
            general.screens.mainScrValues.temp = 256 - general.screens.mainScrValues.crashes[TempCrash];
            mainScrL19Temp5.str = L"эксплуатации -%d*C";
          } else {
            general.screens.mainScrValues.temp = general.screens.mainScrValues.crashes[TempCrash] - 256;
            mainScrL19Temp5.str = L"эксплуатации +%d*C";
          }
          ShowString(&mainScreen, 18, showOrder[3]);
          ShowString(&mainScreen, 19, showOrder[3]);
        }
      }
      if (general.screens.mainScrValues.crashes[ShortCircuitCrash]) {
        ShowString(&mainScreen, 20, showOrder[6]);
        ShowString(&mainScreen, 21, showOrder[6]);
      }
    }
    lastShow = show;
  }
  general.screens.lastState = general.screens.currentScreenState;
  UpdateScreenValues(&mainScreen);
}
