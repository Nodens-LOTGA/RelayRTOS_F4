#include "bitmaps.h"
#include "colors.h"
#include "screens.h"
#include "settings.h"

Screen screenSaver;

static uint16_t displayTime = 0;
uint16_t *screenSaverTimeParam[] = {&displayTime};
Label screenSaverL0Time = CREATE_LABEL_P(70, 60, L"%d", 1, 0, &arialBlack_24ptFontInfo, &blueColor, screenSaverTimeParam);
Label screenSaverL1Reset = CREATE_LABEL(4, 5, L"Сброс настроек", 1, 0, &arialBlack_12ptFontInfo, &blueColor);
Label screenSaverL2Reset = CREATE_LABEL(4, 24, L"к заводским", 1, 0, &arialBlack_12ptFontInfo, &blueColor);

Label *screenSaverLabels[] = {&screenSaverL0Time, &screenSaverL1Reset, &screenSaverL2Reset};

uint8_t screenSaverStrCount = sizeof(screenSaverLabels) / sizeof(screenSaverLabels[0]);

void InitScreenSaver(void) {
  screenSaver.labels = screenSaverLabels;
  screenSaver.strCount = screenSaverStrCount;
}
void EnableScreenSaver(void) {
  screenSaver.bgColor = bgColors[general.settings.bgColorType];
  general.screens.lastState = -1;
  general.screens.lastScreen = general.screens.currentScreen;
  DisableStrings(&screenSaver, 0, screenSaverStrCount);
  ShowScreen(&screenSaver, 1);
  if (general.screens.currentScreenState == 1) {
    ShowString(&screenSaver, 0, 1);
    ShowString(&screenSaver, 1, 1);
    ShowString(&screenSaver, 2, 1);
  }
}

void ShowScreenSaver(void (*delay)(uint32_t), uint32_t (*tick)(void)) {
  if (general.screens.currentScreenState == 0) {
    uint32_t elapsedTicks = 0;
    uint32_t previousTick = tick();
    for (int i = 0; i < sv_Bitmaps.count; i++) {
      ShowBitmapFromFlash(0, 0, sv_Bitmaps.width, sv_Bitmaps.height, sv_Bitmaps.addr[i % sv_Bitmaps.count], 1);
      uint32_t currentTick = tick();
      elapsedTicks = currentTick - previousTick;
      previousTick = currentTick;
      if (elapsedTicks < 20)
        delay(20 - elapsedTicks);
    }
  } else {
    for (displayTime = 5; displayTime > 0; displayTime--) {
      UpdateScreenValues(&screenSaver);
      delay(1000);
    }
  }
}