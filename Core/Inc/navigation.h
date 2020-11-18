#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "screens.h"
#include "setupscreen.h"

typedef enum {
	BTN_NONE = 0,
	BTN_INCREMENT = 1,
	BTN_DECREMENT = 2,
	BTN_ACTION_COUNT
} btn_action;

void NavigateSettingsScreen(void);
void NavigateMainScreen(void);
void NavigateErrorScreen(void);
void NavigateAfterErrorScreen(void);
void NavigateShared(void);
uint8_t IncSetSettingsScreen(uint8_t currentSet);
uint8_t DecSetSettingsScreen(uint8_t currentSet);
void ProcessIncDecBtn(void);

#endif //INTERRUPTS_H