#ifndef SCREENS_H
#define SCREENS_H

#include "display.h"
#include "fonts.h"
#include "wchar.h"

#define CREATE_LABEL_A(X, Y, TEXT, SPACING, OBLIQUE, FONT, COLOR, ALIGN)                                                                                       \
  { X, Y, TEXT, sizeof(TEXT) / sizeof(TEXT[0]) - 1, 0, SPACING, OBLIQUE, 0, FONT, COLOR, 0, 0, ALIGN }
#define CREATE_LABEL_P_A(X, Y, TEXT, SPACING, OBLIQUE, FONT, COLOR, PARAMS, ALIGN)                                                                             \
  { X, Y, TEXT, 0, 0, SPACING, OBLIQUE, 0, FONT, COLOR, sizeof(PARAMS) / sizeof(PARAMS[0]), PARAMS, ALIGN }
#define CREATE_LABEL(X, Y, TEXT, SPACING, OBLIQUE, FONT, COLOR)                                                                                                \
  { X, Y, TEXT, sizeof(TEXT) / sizeof(TEXT[0]) - 1, 0, SPACING, OBLIQUE, 0, FONT, COLOR, 0, 0, TextAlignLeft }
#define CREATE_LABEL_P(X, Y, TEXT, SPACING, OBLIQUE, FONT, COLOR, PARAMS)                                                                                      \
  { X, Y, TEXT, 0, 0, SPACING, OBLIQUE, 0, FONT, COLOR, sizeof(PARAMS) / sizeof(PARAMS[0]), PARAMS, TextAlignLeft }

typedef struct {
  uint8_t x;
  uint8_t y;
  wchar_t *str;
  uint8_t strLength;
  uint8_t pixelWidth;
  uint8_t charSpacing;
  uint8_t oblique;
  uint8_t isVisible;
  FontInfo *font;
  Color *color;
  uint8_t intParamsCount;
  uint16_t **intParams;
  TextAlign align;
} Label;

typedef struct {
  Label **labels;
  const Color *bgColor;
  uint8_t strCount;
} Screen;

void ShowScreen(Screen *screen, uint8_t redrawBg);
void ClearScreen(Screen *screen);
void ClearScreenValue(Screen *screen, uint8_t index);
void UpdateScreenValues(Screen *screen);
void UpdateScreenValue(Screen *screen, uint8_t index);
void UpdateString(Screen *screen, uint8_t index);
uint8_t GetStringVisible(Screen *screen, uint8_t index);
void SetStringVisible(Screen *screen, uint8_t index, uint8_t isVisible);
void DisableStrings(Screen *screen, uint8_t first, uint8_t last);
void insertStrToLabel(Screen *screen, int index);
void ShowString(Screen *screen, uint8_t index, uint8_t show);

extern Screen screenSaver;

void InitScreenSaver(void);
void EnableScreenSaver(void);
void ShowScreenSaver(void (*delay)(uint32_t), uint32_t (*tick)(void));

extern Screen mainScreen;

void InitMainScreen(void);
void EnableMainScrSet(void);
void UpdateMainScrSet(uint32_t tickCount);

extern Screen afterErrScreen;

void InitAfterErrScreen(void);
void EnableAfterErrorScrSet(void);
void UpdateAfterErrorScrSet(void);

extern Screen errorsScreen;

void InitErrorsScreen(void);
void EnableErrorScrSet(void);
void UpdateErrorScrSet(void);

#endif // SCREENS_H