#ifndef DISPLAY_H
#define DISPLAY_H

#include "fonts.h"
#include "main.h"

#define D_PORT DB0_GPIO_Port
#define D_PIN_START DB0_Pin_Num

#define RD_PORT DISP_RD_GPIO_Port
#define RD_PIN DISP_RD_Pin

#define WR_PORT DISP_WR_GPIO_Port
#define WR_PIN DISP_WR_Pin

#define D_C_PORT DISP_RS_GPIO_Port
#define D_C_PIN DISP_RS_Pin

#define RES_PORT DISP_RES_GPIO_Port
#define RES_PIN DISP_RES_Pin

#define CS_PORT DISP_CS_GPIO_Port
#define CS_PIN DISP_CS_Pin

#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 128

extern uint16_t frameLineBuffer[DISPLAY_WIDTH];

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} Color;

typedef struct {
  uint8_t x;
  uint8_t y;
} Point;

typedef enum { TextAlignLeft, TextAlignRight } TextAlign;

void WriteByte(uint8_t data);
void WriteShort(uint8_t high, uint8_t low);
void WriteByteCommand(uint8_t command);
void WriteByteData(uint8_t data);
void InitDisplayPorts(void);
void InitDisplay(void (*delay)(uint32_t));
void SetMemoryAddress(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void FillBackground(Color color);
uint16_t DrawUTF8String(uint16_t x, uint16_t y, const wchar_t *str, uint8_t len, uint8_t charSpacing, uint8_t obliquePercent, Color charColor,
                        FontInfo *fontInfo, TextAlign align);
void DrawChar(uint16_t x, uint16_t y, uint16_t charCode, Color charColor, FontInfo *fontInfo);
void DrawShiftXChar(uint16_t x, uint16_t y, uint8_t percent, uint16_t charCode, Color charColor, FontInfo *fontInfo);
void DrawSpace(uint16_t x, uint16_t y, uint8_t width, uint8_t height);
void DrawShiftXSpace(uint16_t x, uint16_t y, uint8_t percent, uint8_t width, uint8_t height);
void EraseRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Color color);
void DrawBitmap(uint16_t x, uint16_t y, uint8_t width, uint8_t height, const uint16_t *bmpBuff);
void ShowBitmap(uint16_t x, uint16_t y, uint8_t width, uint8_t height, const uint16_t *bmpBuff, uint8_t show);
void ShowBitmapFromFlash(uint16_t x, uint16_t y, uint8_t width, uint8_t height, uint32_t addr, uint8_t show);
void DisplaySetBrightness(uint8_t brightness);

static inline void SetCSHigh(void) { CS_PORT->BSRR = CS_PIN; }

static inline void SetCSLow(void) { CS_PORT->BSRR = CS_PIN << 16U; }

static inline void SetRESHigh(void) { RES_PORT->BSRR = RES_PIN; }

static inline void SetRESLow(void) { RES_PORT->BSRR = RES_PIN << 16U; }

#endif // DISPLAY_H
