#ifndef SETUPSCREEN_H
#define SETUPSCREEN_H

#include "screens.h"
#include "settings.h"

typedef enum {
  MAX_VOLTAGE = 0,
  MIN_VOLTAGE = 1,
  MAX_CURRENT = 2,
  MAX_KW = 3,
  ANY_FREQ = 4,
  ANY_VOLTAGE = 5,
  IRREG_SIN = 6,
  PERCENT_IRREG_SIN = 7,
  DELAY_START = 8,
  TYPE_LOAD = 9,
  SET_TIME = 10,
  START_LIGHT_TIME = 11,
  DURATION_LIGHT = 12,
  COLOR_BG = 13,
  SETUP_SCR_SET_COUNT = 14
} setup_scr_set;

extern Screen setupScreen;

void InitSetupScreen(void);
void EnableSetupScrSet(void);
void UpdateSetupScrSet(uint32_t tickCount);

uint8_t GetSetIndex(uint8_t currentSet);
#endif //SETUPSCREEN_H
