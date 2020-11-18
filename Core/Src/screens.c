#include "screens.h"
#include "display.h"
#include "settings.h"

#define LABEL(screen, index) (*(screen->labels + index))

#define MAX_STR_LENGTH 32

wchar_t strBuffer[MAX_STR_LENGTH];

void ShowScreen(Screen *screen, uint8_t redrawBg) {
  SetCSLow();

  int index = 0;
  if (redrawBg)
    FillBackground(*(screen->bgColor));

  while (index < screen->strCount) {
    if (LABEL(screen, index)->isVisible)
      insertStrToLabel(screen, index);
    index++;
  }

  SetCSHigh();
}

void ClearScreen(Screen *screen) {
  int index = 0;
  while (index < screen->strCount) {
    ClearScreenValue(screen, index);
    index++;
  }
}

void ClearScreenValue(Screen *screen, uint8_t index) {
  if (LABEL(screen, index)->isVisible) {
    SetCSLow();
    if (LABEL(screen, index)->align == TextAlignLeft) {
      if (LABEL(screen, index)->oblique == 0)
        DrawSpace(LABEL(screen, index)->x, LABEL(screen, index)->y, LABEL(screen, index)->pixelWidth, LABEL(screen, index)->font->charHeight);
      else
        DrawShiftXSpace(LABEL(screen, index)->x, LABEL(screen, index)->y, LABEL(screen, index)->oblique, LABEL(screen, index)->pixelWidth,
                        LABEL(screen, index)->font->charHeight);
    } else if (LABEL(screen, index)->align == TextAlignRight) {
      if (LABEL(screen, index)->oblique == 0)
        DrawSpace(LABEL(screen, index)->x + LABEL(screen, index)->pixelWidth, LABEL(screen, index)->y, -LABEL(screen, index)->pixelWidth,
                  LABEL(screen, index)->font->charHeight);
      else
        DrawShiftXSpace(LABEL(screen, index)->x + LABEL(screen, index)->pixelWidth, LABEL(screen, index)->y, LABEL(screen, index)->oblique,
                        -LABEL(screen, index)->pixelWidth, LABEL(screen, index)->font->charHeight);
    }
    SetCSHigh();
  }
}

void UpdateScreenValues(Screen *screen) {
  int index = 0;
  while (screen->strCount > index) {
    UpdateScreenValue(screen, index);
    index++;
  }
}

void UpdateScreenValue(Screen *screen, uint8_t index) {
  if (index >= screen->strCount)
    return;
  if ((*(screen->labels + index))->isVisible == 0)
    return;

  SetCSLow();

  insertStrToLabel(screen, index);

  SetCSHigh();
}

void UpdateString(Screen *screen, uint8_t index) {
  int16_t oldPixelWidth = LABEL(screen, index)->pixelWidth;

  LABEL(screen, index)->pixelWidth = DrawUTF8String(LABEL(screen, index)->x, LABEL(screen, index)->y, (wchar_t *)&strBuffer, LABEL(screen, index)->strLength,
                                                    LABEL(screen, index)->charSpacing, LABEL(screen, index)->oblique, *LABEL(screen, index)->color,
                                                    LABEL(screen, index)->font, LABEL(screen, index)->align);

  if (LABEL(screen, index)->align == TextAlignLeft) {
    if (LABEL(screen, index)->pixelWidth < oldPixelWidth) {
      if (LABEL(screen, index)->oblique == 0)
        DrawSpace(LABEL(screen, index)->x + LABEL(screen, index)->pixelWidth, LABEL(screen, index)->y, oldPixelWidth - LABEL(screen, index)->pixelWidth,
                  LABEL(screen, index)->font->charHeight);
      else
        DrawShiftXSpace(LABEL(screen, index)->x + LABEL(screen, index)->pixelWidth, LABEL(screen, index)->y, LABEL(screen, index)->oblique,
                        oldPixelWidth - LABEL(screen, index)->pixelWidth, LABEL(screen, index)->font->charHeight);
    }
  } else if (LABEL(screen, index)->align == TextAlignRight) {
    if (LABEL(screen, index)->pixelWidth > oldPixelWidth && oldPixelWidth > 0) {
      if (LABEL(screen, index)->oblique == 0)
        DrawSpace(LABEL(screen, index)->x + oldPixelWidth, LABEL(screen, index)->y, -oldPixelWidth + LABEL(screen, index)->pixelWidth,
                  LABEL(screen, index)->font->charHeight);
      else
        DrawShiftXSpace(LABEL(screen, index)->x + oldPixelWidth, LABEL(screen, index)->y, LABEL(screen, index)->oblique,
                        -oldPixelWidth + LABEL(screen, index)->pixelWidth, LABEL(screen, index)->font->charHeight);
    }
  }
}

uint8_t GetStringVisible(Screen *screen, uint8_t index) {
  if (index >= screen->strCount)
    return 0;
  return LABEL(screen, index)->isVisible;
}

void SetStringVisible(Screen *screen, uint8_t index, uint8_t isVisible) {
  if (index >= screen->strCount)
    return;
  LABEL(screen, index)->isVisible = isVisible;
}

void DisableStrings(Screen *screen, uint8_t first, uint8_t last) {
  int i = first;
  for (; i < last; i++) {
    SetStringVisible(screen, i, 0);
  }
}

void ShowString(Screen *screen, uint8_t index, uint8_t show) {
  if (!show) {
    ClearScreenValue(screen, index);
    LABEL(screen, index)->isVisible = 0;
  } else
    LABEL(screen, index)->isVisible = 1;
}

void insertStrToLabel(Screen *screen, int index) {
  switch (LABEL(screen, index)->intParamsCount) {
  case 0: {
    LABEL(screen, index)->pixelWidth = DrawUTF8String(LABEL(screen, index)->x, LABEL(screen, index)->y, LABEL(screen, index)->str,
                                                      LABEL(screen, index)->strLength, LABEL(screen, index)->charSpacing, LABEL(screen, index)->oblique,
                                                      *LABEL(screen, index)->color, LABEL(screen, index)->font, LABEL(screen, index)->align);
    break;
  }
  case 1: {
    LABEL(screen, index)->strLength = swprintf(strBuffer, MAX_STR_LENGTH, LABEL(screen, index)->str, *(*(LABEL(screen, index)->intParams)));
    UpdateString(screen, index);
    break;
  }
  case 2: {
    LABEL(screen, index)->strLength =
        swprintf(strBuffer, MAX_STR_LENGTH, LABEL(screen, index)->str, *(*(LABEL(screen, index)->intParams)), *(*(LABEL(screen, index)->intParams + 1)));

    UpdateString(screen, index);
    break;
  }
  case 3: {
    LABEL(screen, index)->strLength = swprintf(strBuffer, MAX_STR_LENGTH, LABEL(screen, index)->str, *(*(LABEL(screen, index)->intParams)),
                                               *(*(LABEL(screen, index)->intParams + 1)), *(*(LABEL(screen, index)->intParams + 2)));

    UpdateString(screen, index);
    break;
  }
  case 4: {
    LABEL(screen, index)->strLength =
        swprintf(strBuffer, MAX_STR_LENGTH, LABEL(screen, index)->str, *(*(LABEL(screen, index)->intParams)), *(*(LABEL(screen, index)->intParams + 1)),
                 *(*(LABEL(screen, index)->intParams + 2)), *(*(LABEL(screen, index)->intParams + 3)));

    UpdateString(screen, index);
    break;
  }
  case 5: {
    LABEL(screen, index)->strLength =
        swprintf(strBuffer, MAX_STR_LENGTH, LABEL(screen, index)->str, *(*(LABEL(screen, index)->intParams)), *(*(LABEL(screen, index)->intParams + 1)),
                 *(*(LABEL(screen, index)->intParams + 2)), *(*(LABEL(screen, index)->intParams + 3)), *(*(LABEL(screen, index)->intParams + 4)));

    UpdateString(screen, index);
    break;
  }
  default:
    break;
  }
}