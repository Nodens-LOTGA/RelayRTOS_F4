#include "display.h"
#include "main.h"
#include "w25qxx.h"

uint8_t bgColorHigh = 0;
uint8_t bgColorLow = 0;

uint16_t frameLineBuffer[DISPLAY_WIDTH];

void someDelay(void) {
  volatile int i = 7 ;
  while (i--)
    ;
}

__STATIC_INLINE void PulseHigh(GPIO_TypeDef *port, uint16_t pin) {
  port->BSRR = pin;
  asm("nop");
  port->BSRR = pin << 16;
}

__STATIC_INLINE void PulseLow(GPIO_TypeDef *port, uint16_t pin) {
  port->BSRR = pin << 16;
  someDelay();
  port->BSRR = pin;
}

/*__STATIC_INLINE void SetDataReadMode(void) {
  D_PORT->CRH &= ~(GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1 | GPIO_CRH_CNF12_1 | GPIO_CRH_CNF13_1 | GPIO_CRH_CNF14_1 |
                   GPIO_CRH_CNF15_1 | GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 | GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1 |
                   GPIO_CRH_MODE11_0 | GPIO_CRH_MODE11_1 | GPIO_CRH_MODE12_0 | GPIO_CRH_MODE12_1 | GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1 | GPIO_CRH_MODE14_0 |
                   GPIO_CRH_MODE14_1 | GPIO_CRH_MODE15_0 | GPIO_CRH_MODE15_1);

  D_PORT->CRH |=
      (GPIO_CRH_CNF8_0 | GPIO_CRH_CNF9_0 | GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0 | GPIO_CRH_CNF12_0 | GPIO_CRH_CNF13_0 | GPIO_CRH_CNF14_0 | GPIO_CRH_CNF15_0);
}*/

__STATIC_INLINE void SetDataWriteMode(void) {

  /*D_PORT->CRH &= ~(GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1 |
                         GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1 |
                         GPIO_CRH_CNF12_1 | GPIO_CRH_CNF13_1 |
                         GPIO_CRH_CNF14_1 | GPIO_CRH_CNF15_1);

                     D_PORT->CRH |= (GPIO_CRH_CNF8_0 | GPIO_CRH_CNF9_0 |
                                     GPIO_CRH_CNF10_0 | GPIO_CRH_CNF11_0 |
                                     GPIO_CRH_CNF12_0 | GPIO_CRH_CNF13_0 |
                                     GPIO_CRH_CNF14_0 | GPIO_CRH_CNF15_0 |
                                     GPIO_CRH_MODE8_0 | GPIO_CRH_MODE8_1 |
                                     GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 |
                                     GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1 |
                                     GPIO_CRH_MODE11_0 | GPIO_CRH_MODE11_1 |
                                     GPIO_CRH_MODE12_0 | GPIO_CRH_MODE12_1 |
                                     GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1 |
                                     GPIO_CRH_MODE14_0 | GPIO_CRH_MODE14_1 |
                                     GPIO_CRH_MODE15_0 | GPIO_CRH_MODE15_1);*/
}

void WriteShort(uint8_t high, uint8_t low) {
  uint32_t bsrrLow = ((uint32_t)low << D_PIN_START) + ((uint32_t)(~low) << D_PIN_START << 16U);
  uint32_t bsrrHigh = ((uint32_t)high << D_PIN_START) + ((uint32_t)(~high) << D_PIN_START << 16U);
  D_PORT->BSRR = bsrrHigh;
  PulseLow(WR_PORT, WR_PIN);
  D_PORT->BSRR = bsrrLow;
  PulseLow(WR_PORT, WR_PIN);
}

void WriteByte(uint8_t data) {
  volatile uint32_t dat = ((uint32_t)data << D_PIN_START) + ((uint32_t)(~data) << D_PIN_START << 16U);
  D_PORT->BSRR = ((uint32_t)data << D_PIN_START) + ((uint32_t)(~data) << D_PIN_START << 16U);
  PulseLow(WR_PORT, WR_PIN);
}

void WriteByteCommand(uint8_t command) {
  D_C_PORT->BSRR = D_C_PIN << 16U;
  WriteByte(command);
}

void WriteByteData(uint8_t data) {
  D_C_PORT->BSRR = D_C_PIN;
  WriteByte(data);
}

void WriteShortData(uint16_t data) {
  D_C_PORT->BSRR = D_C_PIN;
  WriteShort((uint8_t)(data >> 8), (uint8_t)data);
}

void InitDisplayPorts(void) {
  RES_PORT->BSRR = RES_PIN;
  CS_PORT->BSRR = CS_PIN;
  RD_PORT->BSRR = RD_PIN;
  WR_PORT->BSRR = WR_PIN;
  D_C_PORT->BSRR = D_C_PIN << 16U;
  SetDataWriteMode();
}

void InitDisplay(void (*delay)(uint32_t)) {
  InitDisplayPorts();

  SetRESLow();
  (*delay)(100);
  SetRESHigh();

  SetCSLow();

  WriteByteCommand(0x01);
  (*delay)(120);

  WriteByteCommand(0x11);
  (*delay)(120);

  // ST7735R Frame Rate
  WriteByteCommand(0xB1);
  WriteByteData(0x01);
  WriteByteData(0x2C);
  WriteByteData(0x2D);
  WriteByteCommand(0xB2);
  WriteByteData(0x01);
  WriteByteData(0x2C);
  WriteByteData(0x2D);
  WriteByteCommand(0xB3);
  WriteByteData(0x01);
  WriteByteData(0x2C);
  WriteByteData(0x2D);
  WriteByteData(0x01);
  WriteByteData(0x2C);
  WriteByteData(0x2D);

  WriteByteCommand(0xB4); // Column inversion
  WriteByteData(0x07);

  // ST7735R Power Sequence
  WriteByteCommand(0xC0);
  WriteByteData(0xA2);
  WriteByteData(0x02);
  WriteByteData(0x84);
  WriteByteCommand(0xC1);
  WriteByteData(0xC5);
  WriteByteCommand(0xC2);
  WriteByteData(0x0A);
  WriteByteData(0x00);
  WriteByteCommand(0xC3);
  WriteByteData(0x8A);
  WriteByteData(0x2A);
  WriteByteCommand(0xC4);
  WriteByteData(0x8A);
  WriteByteData(0xEE);

  WriteByteCommand(0xC5); // VCOM
  WriteByteData(0x0E);

  WriteByteCommand(0x20);

  WriteByteCommand(0x36); // MX, MY, RGB mode
  WriteByteData(0xAC);

  // ST7735R Gamma Sequence
  WriteByteCommand(0xE0);
  WriteByteData(0x0f);
  WriteByteData(0x1a);
  WriteByteData(0x0f);
  WriteByteData(0x18);
  WriteByteData(0x2f);
  WriteByteData(0x28);
  WriteByteData(0x20);
  WriteByteData(0x22);
  WriteByteData(0x1f);
  WriteByteData(0x1b);
  WriteByteData(0x23);
  WriteByteData(0x37);
  WriteByteData(0x00);
  WriteByteData(0x07);
  WriteByteData(0x02);
  WriteByteData(0x10);
  WriteByteCommand(0xe1);
  WriteByteData(0x0f);
  WriteByteData(0x1b);
  WriteByteData(0x0f);
  WriteByteData(0x17);
  WriteByteData(0x33);
  WriteByteData(0x2c);
  WriteByteData(0x29);
  WriteByteData(0x2e);
  WriteByteData(0x30);
  WriteByteData(0x30);
  WriteByteData(0x39);
  WriteByteData(0x3f);
  WriteByteData(0x00);
  WriteByteData(0x07);
  WriteByteData(0x03);
  WriteByteData(0x10);

  WriteByteCommand(0x2a);
  WriteByteData(0x00);
  WriteByteData(0x00);
  WriteByteData(0x00);
  WriteByteData(0x7f);
  WriteByteCommand(0x2b);
  WriteByteData(0x00);
  WriteByteData(0x00);
  WriteByteData(0x00);
  WriteByteData(0x9f);

  WriteByteCommand(0xF0); // Enable test command
  WriteByteData(0x01);
  WriteByteCommand(0xF6); // Disable ram power save mode
  WriteByteData(0x00);

  WriteByteCommand(0x3A); // 65k mode
  WriteByteData(0x05);
  WriteByteCommand(0x13); // Normal display mode on
  (*delay)(10);
  WriteByteCommand(0x29); // Display on
  (*delay)(100);
  SetCSHigh();
  (*delay)(100);
}

void SetMemoryAddress(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
  WriteByteCommand(0x2A); // Column address set
  WriteShortData(xStart);
  WriteShortData(xEnd);

  WriteByteCommand(0x2B); // Row address set
  WriteShortData(yStart);
  WriteShortData(yEnd);
}

void FillBackground(Color color) {
  bgColorHigh = (color.red & 0xF8) + (color.green >> 5U);
  bgColorLow = ((color.green & 0x1C) << 3) + (color.blue >> 3U);
  SetDataWriteMode();

  SetMemoryAddress(0, 0, 159, 127);

  WriteByteCommand(0x2C);
  for (int i = 0; i < 160; i++) {
    for (int j = 0; j < 128; j++) {
      WriteByteData(bgColorHigh);
      WriteByteData(bgColorLow);
    }
  }
}

uint16_t DrawUTF8String(uint16_t x, uint16_t y, const wchar_t *str, uint8_t len, uint8_t charSpacing, uint8_t obliquePercent, Color charColor,
                        FontInfo *fontInfo, TextAlign align) {
  int16_t width = 0;

  // if (y + fontInfo->charHeight > DISPLAY_HEIGHT) return 0;

  if (align == TextAlignLeft) {
    for (int i = 0; i < len; i++) {
      switch (str[i]) {
      case '*':
        // if ((x + width + fontInfo->spaceCharWidth) > DISPLAY_WIDTH) return width;
        DrawRectangle(x + width + (fontInfo->spaceCharWidth / 4), y + ((5 * fontInfo->charHeight) / 12), x + width + ((3 * fontInfo->spaceCharWidth) / 4),
                      y + ((7 * fontInfo->charHeight) / 12), charColor);
        width += fontInfo->spaceCharWidth + charSpacing;
        break;
      case ' ':
        // if ((x + width + fontInfo->spaceCharWidth) > DISPLAY_WIDTH) return width;
        if (obliquePercent == 0)
          DrawSpace(x + width, y, fontInfo->spaceCharWidth, fontInfo->charHeight);
        else
          DrawShiftXSpace(x + width, y, obliquePercent, fontInfo->spaceCharWidth, fontInfo->charHeight);
        width += fontInfo->spaceCharWidth + charSpacing;
        break;
      default:
        if (fontInfo->blocksCount != 0) {
          uint8_t isDescriptorFinded = 0;
          for (int j = 0; j < fontInfo->blocksCount; j++) {
            if (str[i] >= fontInfo->blocks[j].startCharCode && str[i] <= fontInfo->blocks[j].endCharCode) {
              isDescriptorFinded = 1;
              fontInfo->charBlock = fontInfo->blocks[j];
            }
          }
          if (isDescriptorFinded == 0) {
            continue;
          }
        }
        // if ((x + width + fontInfo->charBlock.charDescriptoprs[(str[i] - fontInfo->charBlock.startCharCode) * 2]) > DISPLAY_WIDTH) return width;
        if (obliquePercent == 0)
          DrawChar(x + width, y, str[i], charColor, fontInfo);
        else
          DrawShiftXChar(x + width, y, obliquePercent, str[i], charColor, fontInfo);
        width += (fontInfo->charBlock.charDescriptoprs[(str[i] - fontInfo->charBlock.startCharCode) * 2]) + charSpacing;
        break;
      }

      if (obliquePercent == 0)
        DrawSpace(x + width - charSpacing, y, charSpacing, fontInfo->charHeight);
      else
        DrawShiftXSpace(x + width - charSpacing, y, obliquePercent, charSpacing, fontInfo->charHeight);
    }
  } else if (align == TextAlignRight) {
    for (int i = len - 1; i >= 0; i--) {
      switch (str[i]) {
      case '*':
        // if ((x + width + fontInfo->spaceCharWidth) > DISPLAY_WIDTH) return width;
        width -= fontInfo->spaceCharWidth + charSpacing;
        DrawRectangle(x + width + (fontInfo->spaceCharWidth / 4), y + ((5 * fontInfo->charHeight) / 12), x + width + ((3 * fontInfo->spaceCharWidth) / 4),
                      y + ((7 * fontInfo->charHeight) / 12), charColor);
        break;
      case ' ':
        // if ((x + width + fontInfo->spaceCharWidth) > DISPLAY_WIDTH) return width;
        width -= fontInfo->spaceCharWidth + charSpacing;
        if (obliquePercent == 0)
          DrawSpace(x + width, y, fontInfo->spaceCharWidth, fontInfo->charHeight);
        else
          DrawShiftXSpace(x + width, y, obliquePercent, fontInfo->spaceCharWidth, fontInfo->charHeight);
        break;
      default:
        if (fontInfo->blocksCount != 0) {
          uint8_t isDescriptorFinded = 0;
          for (int j = 0; j < fontInfo->blocksCount; j++) {
            if (str[i] >= fontInfo->blocks[j].startCharCode && str[i] <= fontInfo->blocks[j].endCharCode) {
              isDescriptorFinded = 1;
              fontInfo->charBlock = fontInfo->blocks[j];
            }
          }
          if (isDescriptorFinded == 0) {
            continue;
          }
        }
        // if ((x + width + fontInfo->charBlock.charDescriptoprs[(str[i] - fontInfo->charBlock.startCharCode) * 2]) > DISPLAY_WIDTH) return width;
        width -= (fontInfo->charBlock.charDescriptoprs[(str[i] - fontInfo->charBlock.startCharCode) * 2]) + charSpacing;
        if (obliquePercent == 0)
          DrawChar(x + width, y, str[i], charColor, fontInfo);
        else
          DrawShiftXChar(x + width, y, obliquePercent, str[i], charColor, fontInfo);
        break;
      }

      if (obliquePercent == 0)
        DrawSpace(x + width - charSpacing, y, charSpacing, fontInfo->charHeight);
      else
        DrawShiftXSpace(x + width - charSpacing, y, obliquePercent, charSpacing, fontInfo->charHeight);
    }
  }
  return width;
}

void DrawChar(uint16_t x, uint16_t y, uint16_t charCode, Color charColor, FontInfo *fontInfo) {
  if (charCode < fontInfo->charBlock.startCharCode || charCode > fontInfo->charBlock.endCharCode)
    return;
  uint8_t charHeight = fontInfo->charHeight;
  uint16_t *ptr = fontInfo->charBlock.charDescriptoprs + ((charCode - fontInfo->charBlock.startCharCode) * 2);
  uint8_t charWidth = fontInfo->charBlock.charDescriptoprs[(charCode - fontInfo->charBlock.startCharCode) * 2];

  // if (x + charWidth > DISPLAY_WIDTH || y + charHeight > DISPLAY_HEIGHT) return;

  uint8_t *startPtr = fontInfo->charBitmaps + fontInfo->charBlock.charDescriptoprs[(charCode - fontInfo->charBlock.startCharCode) * 2 + 1];
  uint8_t charColorHigh = (charColor.red & 0xF8) + (charColor.green >> 5U);
  uint8_t charColorLow = ((charColor.green & 0x1C) << 3) + (charColor.blue >> 3U);
  uint8_t charWidthInBytes = (charWidth / 8) + ((charWidth % 8) != 0 ? 1 : 0);
  SetMemoryAddress(x, y, x + charWidth - 1, y + charHeight - 1);
  WriteByteCommand(0x2C);

  for (int i = 0; i < charHeight; i++) {
    if (y + i >= DISPLAY_HEIGHT)
      break;
    for (int j = 0; j < charWidth; j++) {
      if (x + j >= DISPLAY_WIDTH)
        break;
      uint8_t charBitmap = *(startPtr + (i * charWidthInBytes) + (j / 8));
      if (charBitmap & (0x80 >> (j % 8))) {
        WriteByteData(charColorHigh);
        WriteByteData(charColorLow);
      } else {
        WriteByteData(bgColorHigh);
        WriteByteData(bgColorLow);
      }
    }
  }
}

void DrawShiftXChar(uint16_t x, uint16_t y, uint8_t percent, uint16_t charCode, Color charColor, FontInfo *fontInfo) {
  if (charCode < fontInfo->charBlock.startCharCode || charCode > fontInfo->charBlock.endCharCode)
    return;
  uint8_t charHeight = fontInfo->charHeight;
  uint16_t *ptr = fontInfo->charBlock.charDescriptoprs + ((charCode - fontInfo->charBlock.startCharCode) * 2);
  uint8_t charWidth = fontInfo->charBlock.charDescriptoprs[(charCode - fontInfo->charBlock.startCharCode) * 2];

  uint16_t charShiftWidth = ((uint16_t)fontInfo->charBlock.charDescriptoprs[(charCode - fontInfo->charBlock.startCharCode) * 2]) +
                            (((uint16_t)charHeight) * ((uint16_t)percent) / 100);

  // if (x + charShiftWidth > DISPLAY_WIDTH || y + charHeight > DISPLAY_HEIGHT) return;

  uint8_t *startPtr = fontInfo->charBitmaps + fontInfo->charBlock.charDescriptoprs[(charCode - fontInfo->charBlock.startCharCode) * 2 + 1];
  uint8_t charColorHigh = (charColor.red & 0xF8) + (charColor.green >> 5U);
  uint8_t charColorLow = ((charColor.green & 0x1C) << 3) + (charColor.blue >> 3U);
  uint8_t charWidthInBytes = (charWidth / 8) + ((charWidth % 8) != 0 ? 1 : 0);

  for (int i = 0; i < charHeight; i++) {
    uint16_t shift = (((uint16_t)charHeight - i) * ((uint16_t)percent) / 100);
    SetMemoryAddress(x + shift, y + i, x + shift + charWidth - 1, y + i);
    WriteByteCommand(0x2C);
    for (int j = 0; j < charWidth; j++) {
      uint8_t charBitmap = *(startPtr + (i * charWidthInBytes) + (j / 8));
      if (charBitmap & (0x80 >> (j % 8))) {
        WriteByteData(charColorHigh);
        WriteByteData(charColorLow);
      } else {
        WriteByteData(bgColorHigh);
        WriteByteData(bgColorLow);
      }
    }
  }
}

void DrawShiftXSpace(uint16_t x, uint16_t y, uint8_t percent, uint8_t width, uint8_t height) {
  uint16_t charShiftWidth = ((uint16_t)width) + (((uint16_t)height) * ((uint16_t)percent) / 100);
  // if (x + charShiftWidth > DISPLAY_WIDTH || y + height > DISPLAY_HEIGHT) return;
  uint8_t charWidthInBytes = (width / 8) + ((width % 8) != 0 ? 1 : 0);

  for (int i = 0; i < height; i++) {
    uint16_t shift = (((uint16_t)height - i) * ((uint16_t)percent) / 100);
    SetMemoryAddress(x + shift, y + i, x + shift + width - 1, y + i);
    WriteByteCommand(0x2C);
    for (int j = 0; j < width; j++) {
      WriteByteData(bgColorHigh);
      WriteByteData(bgColorLow);
    }
  }
}

void DrawSpace(uint16_t x, uint16_t y, uint8_t width, uint8_t height) {
  // if (x + width > DISPLAY_WIDTH || y + height > DISPLAY_HEIGHT) return;
  EraseRectangle(x, y, x + width, y + height);
}

void EraseRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  if (x1 > x2 || y1 > y2)
    return;
  // if (x2 > DISPLAY_WIDTH || y2 > DISPLAY_HEIGHT) return;

  SetMemoryAddress(x1, y1, x2 - 1, y2 - 1);
  WriteByteCommand(0x2C);

  for (int i = 0; i < y2 - y1; i++) {
    for (int j = 0; j < x2 - x1; j++) {
      WriteByteData(bgColorHigh);
      WriteByteData(bgColorLow);
    }
  }
}

void DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color) {
  if (x1 > x2 || y1 > y2)
    return;
  // if (x2 > DISPLAY_WIDTH || y2 > DISPLAY_HEIGHT) return;

  uint8_t colorHigh = (color.red & 0xF8) + (color.green >> 5U);
  uint8_t colorLow = ((color.green & 0x1C) << 3) + (color.blue >> 3U);

  SetMemoryAddress(x1, y1, x2 - 1, y2 - 1);
  WriteByteCommand(0x2C);

  for (int i = 0; i < y2 - y1; i++) {
    for (int j = 0; j < x2 - x1; j++) {
      WriteByteData(colorHigh);
      WriteByteData(colorLow);
    }
  }
}

void DrawBitmap(uint16_t x, uint16_t y, uint8_t width, uint8_t height, const uint16_t *bmpBuff) {
  SetMemoryAddress(x, y, x + width - 1, y + height - 1);
  WriteByteCommand(0x2C);
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      WriteByteData(bmpBuff[i * height + j] >> 8);
      WriteByteData(bmpBuff[i * height + j] & 0xF0);
    }
  }
}

void ShowBitmap(uint16_t x, uint16_t y, uint8_t width, uint8_t height, const uint16_t *bmpBuff, uint8_t show) {
  if (show) {
    SetCSLow();
    DrawBitmap(x, y, width, height, bmpBuff);
    SetCSHigh();
  } else {
    SetCSLow();
    DrawSpace(x, y, width, height);
    SetCSHigh();
  }
}

void ShowBitmapFromFlash(uint16_t x, uint16_t y, uint8_t width, uint8_t height, uint32_t addr, uint8_t show) {
  if (show) {
    for (int i = 0; i < height; i++) {
      W25qxx_ReadBytes((uint8_t *)frameLineBuffer, addr + i * 2 * width, 2 * width);
      SetCSLow();
      DrawBitmap(x, y + i, width, 1, frameLineBuffer);
      SetCSHigh();
    }
  } else {
    SetCSLow();
    DrawSpace(x, y, width, height);
    SetCSHigh();
  }
}

void DisplaySetBrightness(uint8_t brightness) {
  brightness = (brightness * LED_TIM->ARR) / 100;
  if (brightness == LED_TIM->LED_TIM_CCR)
    return;
  LED_TIM->LED_TIM_CCR = brightness > LED_TIM->ARR ? LED_TIM->ARR : brightness;
}