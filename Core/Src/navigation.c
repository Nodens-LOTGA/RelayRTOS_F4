#include "navigation.h"
#include "tasks.h"
#include "settings.h"
#include "utils.h"

static uint8_t lastButtonState = 0;
static uint8_t isLongPressed = 0;
static uint8_t blockBetweenScr = 0;
static uint8_t blockBetweenSet = 0;
static btn_action buttonAction = BTN_NONE;
static uint32_t lastDelays[BTN_COUNT] = {0, 0, 0};

#define IS_PRESSED(buttonId) (((general.control.btnPressed) & (1 << (buttonId))) == (1 << (buttonId)))
#define IS_NOT_PRESSED(buttonId) (((general.control.btnPressed) & (1 << (buttonId))) == 0)
#define IS_ONLY_PRESSED(buttonId) ((general.control.btnPressed) == (1 << (buttonId)))

static void ProcessLastDelays(uint8_t size, uint32_t *currentDelays, uint32_t *lastDelays) {
  for (uint8_t i = 0; i < size; i++) {
    lastDelays[i] = currentDelays[i];
  }
}

static void ResetDelays(uint8_t size, uint32_t *delays) {
  for (uint8_t i = 0; i < size; i++)
    delays[i] = 0;
}

static void changeScreenTo(uint8_t screen, uint8_t state) {
  ResetDelays(BTN_COUNT, (uint32_t *)&general.control.btnTimePressed);
  ResetDelays(BTN_COUNT, (uint32_t *)&lastDelays);
  blockBetweenScr = 1;
  lastButtonState = 255;
  isLongPressed = 0;
  general.screens.currentScreen = screen;
  general.screens.currentScreenState = state;
}

void NavigateSettingsScreen(void) {
  if (lastButtonState == 0 && general.control.btnPressed == 0)
    return;
  buttonAction = BTN_NONE;
  if (lastButtonState != general.control.btnPressed)
    isLongPressed = 0;
  lastButtonState = general.control.btnPressed;

  if (blockBetweenScr == 1 && general.control.btnPressed != 0)
    return;
  blockBetweenScr = 0;

  if (IS_NOT_PRESSED(2) && IS_PRESSED(1) && IS_PRESSED(0)) {
    if (general.control.btnTimePressed[1] > 1000 && general.control.btnTimePressed[0] > 1000) {
      general.screens.currentScreenState += IncSetSettingsScreen(general.screens.currentScreenState);
      general.screens.setupScrValues.selectedEl = 0;
      blockBetweenSet = 1;
      ResetDelays(BTN_COUNT, (uint32_t *)&general.control.btnTimePressed);
      return;
    }
  }

  if (IS_PRESSED(2) && IS_PRESSED(0) && IS_NOT_PRESSED(1)) {
    if (general.control.btnTimePressed[2] > 1000 && general.control.btnTimePressed[0] > 1000) {
      general.screens.currentScreenState -= DecSetSettingsScreen(general.screens.currentScreenState);
      general.screens.setupScrValues.selectedEl = 0;
      blockBetweenSet = 1;
      ResetDelays(BTN_COUNT, (uint32_t *)&general.control.btnTimePressed);
      return;
    }
  }

  if (blockBetweenSet == 1 && general.control.btnPressed != 0) {
    ResetDelays(BTN_COUNT, (uint32_t *)&lastDelays);
    return;
  }
  blockBetweenSet = 0;

  ProcessIncDecBtn();
  switch (general.screens.currentScreenState) {
  case MAX_VOLTAGE: {
    if (buttonAction == BTN_DECREMENT && general.settings.maxVoltage < 270)
      general.settings.maxVoltage += 5;
    if (buttonAction == BTN_INCREMENT && general.settings.maxVoltage > 245)
      general.settings.maxVoltage -= 5;
    break;
  }
  case MIN_VOLTAGE: {
    if (buttonAction == BTN_DECREMENT && general.settings.minVoltage < 195)
      general.settings.minVoltage += 5;
    if (buttonAction == BTN_INCREMENT && general.settings.minVoltage > 160)
      general.settings.minVoltage -= 5;
    break;
  }
  case MAX_CURRENT: {
    if (buttonAction == BTN_DECREMENT && general.settings.maxCurrent < 63)
      general.settings.maxCurrent += 1;
    if (buttonAction == BTN_INCREMENT && general.settings.maxCurrent > 1)
      general.settings.maxCurrent -= 1;
    break;
  }
  case MAX_KW: {
    if (buttonAction == BTN_DECREMENT && general.settings.maxCurrent < 63)
      general.settings.maxCurrent += 1;
    if (buttonAction == BTN_INCREMENT && general.settings.maxCurrent > 1)
      general.settings.maxCurrent -= 1;
    break;
  }
  case TYPE_LOAD: {
    if (buttonAction != BTN_NONE && general.settings.loadType == 0)
      general.settings.loadType = 1;
    else if (buttonAction != BTN_NONE)
      general.settings.loadType = 0;
    break;
  }
  case SET_TIME: {
    if (buttonAction == BTN_INCREMENT) {
      if (general.screens.setupScrValues.selectedEl == 4)
        general.screens.setupScrValues.selectedEl = 0;
      else
        general.screens.setupScrValues.selectedEl++;
    }
    if (buttonAction == BTN_DECREMENT) {
      general.state.isTimeDateChanged = 1;
      switch (general.screens.setupScrValues.selectedEl) {
      case 0: {
        general.settings.currentDate.hours = (general.settings.currentDate.hours == 23) ? 0 : general.settings.currentDate.hours + 1;
        break;
      }
      case 1: {
        general.settings.currentDate.minutes = (general.settings.currentDate.minutes == 59) ? 0 : general.settings.currentDate.minutes + 1;
        break;
      }
      case 2: {
        general.settings.currentDate.day =
            (general.settings.currentDate.day == GetMaxDay(general.settings.currentDate.month, general.settings.currentDate.year))
                ? 1
                : general.settings.currentDate.day + 1;
        break;
      }
      case 3: {
        general.settings.currentDate.month = (general.settings.currentDate.month == 12) ? 1 : general.settings.currentDate.month + 1;
        uint8_t maxDay = GetMaxDay(general.settings.currentDate.month, general.settings.currentDate.year);
        if (general.settings.currentDate.day > maxDay)
          general.settings.currentDate.day = maxDay;
        break;
      }
      case 4: {
        general.settings.currentDate.year = (general.settings.currentDate.year == 50) ? 20 : general.settings.currentDate.year + 1;
        uint8_t maxDay = GetMaxDay(general.settings.currentDate.month, general.settings.currentDate.year);
        if (general.settings.currentDate.day > maxDay)
          general.settings.currentDate.day = maxDay;
        break;
      }
      default:
        break;
      }
    }
    break;
  }
  case START_LIGHT_TIME: {
    if (buttonAction == BTN_INCREMENT) {
      if (general.settings.lightTimeH == 23)
        general.settings.lightTimeH = 0;
      else
        general.settings.lightTimeH++;
    }
    if (buttonAction == BTN_DECREMENT) {
      if (general.settings.lightTimeM == 59)
        general.settings.lightTimeM = 0;
      else
        general.settings.lightTimeM++;
    }
    break;
  }
  case DURATION_LIGHT: {
    if (buttonAction == BTN_DECREMENT && general.settings.lightDurH < 24)
      general.settings.lightDurH += 1;
    if (buttonAction == BTN_INCREMENT && general.settings.lightDurH > 0)
      general.settings.lightDurH -= 1;
    break;
  }
  case COLOR_BG: {
    if (buttonAction == BTN_DECREMENT)
      general.settings.bgColorType = (general.settings.bgColorType == BgColorCount - 1) ? BgCyanColor : general.settings.bgColorType + 1;
    else if (buttonAction == BTN_INCREMENT)
      general.settings.bgColorType = (general.settings.bgColorType == BgCyanColor) ? BgColorCount - 1 : general.settings.bgColorType - 1;
    break;
  }
  case DELAY_START: {
    if (buttonAction == BTN_DECREMENT && general.settings.startDelayS < 985)
      general.settings.startDelayS += 5;
    if (buttonAction == BTN_INCREMENT && general.settings.startDelayS > 5)
      general.settings.startDelayS -= 5;
    break;
  }
  case IRREG_SIN: {
    if (buttonAction != BTN_NONE && general.settings.thdAllowed == 0)
      general.settings.thdAllowed = 1;
    else if (buttonAction != 0)
      general.settings.thdAllowed = 0;
    break;
  }
  case PERCENT_IRREG_SIN: {
    if (buttonAction == BTN_DECREMENT && general.settings.thdPercent < 100)
      general.settings.thdPercent += 1;
    if (buttonAction == BTN_INCREMENT && general.settings.thdPercent > 1)
      general.settings.thdPercent -= 1;
    break;
  }
  case ANY_FREQ: {
    if (buttonAction != BTN_NONE && general.settings.freqIrrAllowed == 0)
      general.settings.freqIrrAllowed = 1;
    else if (buttonAction != BTN_NONE)
      general.settings.freqIrrAllowed = 0;
    break;
  }
  case ANY_VOLTAGE: {
    if (buttonAction != BTN_NONE && general.settings.voltageIrrAllowed == 0)
      general.settings.voltageIrrAllowed = 1;
    else if (buttonAction != BTN_NONE)
      general.settings.voltageIrrAllowed = 0;
    break;
  }
  default:
    break;
  }
}

uint8_t IncSetSettingsScreen(uint8_t currentSet) {

  switch (currentSet) {
  case MAX_VOLTAGE:
    return 1;
  case MIN_VOLTAGE:
    return 1 + general.settings.loadType;
  case MAX_CURRENT:
    return 2;
  case MAX_KW:
  case ANY_FREQ:
  case ANY_VOLTAGE:
  case IRREG_SIN:
  case PERCENT_IRREG_SIN:
  case DELAY_START:
  case TYPE_LOAD:
  case SET_TIME:
  case START_LIGHT_TIME:
  case DURATION_LIGHT:
    return 1;
  case COLOR_BG:
    changeScreenTo(0, 0);
  default:
    return 0;
  }
}

uint8_t DecSetSettingsScreen(uint8_t currentSet) {

  switch (currentSet) {
  case MAX_VOLTAGE:
    return 0;
  case MIN_VOLTAGE:
  case MAX_CURRENT:
    return 1;
  case MAX_KW:
    return 2;
  case ANY_FREQ:
    return 2 - general.settings.loadType;
  case ANY_VOLTAGE:
  case IRREG_SIN:
  case PERCENT_IRREG_SIN:
  case DELAY_START:
  case TYPE_LOAD:
  case SET_TIME:
  case START_LIGHT_TIME:
  case DURATION_LIGHT:
  case COLOR_BG:
    return 1;
  default:
    return 0;
  }
}

static void ProcessActionBtn(btn_action action, uint8_t btnIndex, uint16_t isPressedTime, uint16_t isLongPressedTime, uint16_t longPressInterval) {
  if (general.control.btnPressed == 0 && lastDelays[btnIndex] > isPressedTime && isLongPressed == 0) {
    uint32_t otherBtn = 0;
    for (int i = 0; i < BTN_COUNT; i++) {
      if (btnIndex == i)
        continue;
      otherBtn += general.control.btnTimePressed[i];
    }
    if (otherBtn == 0) {
      general.control.btnTimePressed[btnIndex] = 0;
      buttonAction = action;
    }
  }
  if (IS_ONLY_PRESSED(btnIndex) && general.control.btnTimePressed[btnIndex] > isLongPressedTime) {
    general.control.btnTimePressed[btnIndex] = 0;
    isLongPressed = 1;
    buttonAction = action;
  }
  if (IS_ONLY_PRESSED(btnIndex) && isLongPressed == 1 && general.control.btnTimePressed[btnIndex] > longPressInterval) {
    general.control.btnTimePressed[btnIndex] = 0;
    buttonAction = action;
  }
}

void ProcessIncDecBtn(void) {
  ProcessActionBtn(BTN_DECREMENT, 1, 100, 1500, 100);
  ProcessActionBtn(BTN_INCREMENT, 0, 100, 1500, 100);
  ProcessLastDelays(BTN_COUNT, (uint32_t *)&general.control.btnTimePressed, (uint32_t *)&lastDelays);
}

void NavigateMainScreen(void) {
  if (lastButtonState == 0 && general.control.btnPressed == 0)
    return;
  buttonAction = BTN_NONE;
  if (lastButtonState != general.control.btnPressed)
    isLongPressed = 0;
  lastButtonState = general.control.btnPressed;
  if (blockBetweenScr == 1 && general.control.btnPressed != 0)
    return;
  blockBetweenScr = 0;
  ProcessActionBtn(BTN_DECREMENT, 1, 100, 10000, 500);
  ProcessActionBtn(BTN_INCREMENT, 0, 100, 20000, 100);
  ProcessLastDelays(BTN_COUNT, (uint32_t *)&general.control.btnTimePressed, (uint32_t *)&lastDelays);
  if (general.control.btnPressed) {
    if (IS_NOT_PRESSED(2) && IS_PRESSED(1) && IS_PRESSED(0)) {
      if (general.control.btnTimePressed[1] >= 1000 && general.control.btnTimePressed[0] >= 1000) {
        changeScreenTo(1, 0);
        return;
      }
    } else if (IS_ONLY_PRESSED(1)) {
      if (buttonAction == BTN_DECREMENT) {
        changeScreenTo(4, 1);
        return;
      }
    } else if (IS_ONLY_PRESSED(0)) {
      if (buttonAction == BTN_INCREMENT) {
        general.state.recalcAdcZero = 1;
        return;
      }
    }
  } else {
    if (buttonAction == BTN_DECREMENT) {
      changeScreenTo(2, 0);
      return;
    }
    if (buttonAction == BTN_INCREMENT) {
      changeScreenTo(3, 0);
      return;
    }
  }
}

void NavigateErrorScreen(void) {
  if (lastButtonState == 0 && general.control.btnPressed == 0)
    return;
  buttonAction = BTN_NONE;
  if (lastButtonState != general.control.btnPressed)
    isLongPressed = 0;
  lastButtonState = general.control.btnPressed;
  if (blockBetweenScr == 1 && general.control.btnPressed != 0)
    return;
  blockBetweenScr = 0;
  ProcessActionBtn(BTN_DECREMENT, 1, 100, 1500, 500);
  ProcessActionBtn(BTN_INCREMENT, 0, 100, 1500, 500);
  ProcessLastDelays(BTN_COUNT, (uint32_t *)&general.control.btnTimePressed, (uint32_t *)&lastDelays);
  // if (general.control.btnPressed == 0) {
  if (buttonAction == BTN_DECREMENT) {
    changeScreenTo(0, 0);
    return;
  } else if (buttonAction == BTN_INCREMENT) {
    general.screens.currentScreenState++;
    if (general.screens.currentScreenState >= general.screens.errorsScrValues.errors[0].value) {
      changeScreenTo(0, 0);
      return;
    }
    ResetDelays(BTN_COUNT, (uint32_t *)&general.control.btnTimePressed);
  }
  //}
}

void NavigateAfterErrorScreen(void) {
  if (lastButtonState == 0 && general.control.btnPressed == 0)
    return;
  buttonAction = BTN_NONE;
  if (lastButtonState != general.control.btnPressed)
    isLongPressed = 0;
  lastButtonState = general.control.btnPressed;
  if (blockBetweenScr == 1 && general.control.btnPressed != 0)
    return;
  blockBetweenScr = 0;
  ProcessActionBtn(BTN_DECREMENT, 0, 100, 1500, 500);
  ProcessActionBtn(BTN_INCREMENT, 1, 100, 1500, 500);
  ProcessLastDelays(BTN_COUNT, (uint32_t *)&general.control.btnTimePressed, (uint32_t *)&lastDelays);
  // if (general.control.btnPressed == 0) {
  if (buttonAction == BTN_DECREMENT) {
    changeScreenTo(0, 0);
    return;
  } else if (buttonAction == BTN_INCREMENT) {
    general.screens.currentScreenState++;
    if (general.screens.currentScreenState > general.screens.crashesScrValues.crashes[0].value) {
      changeScreenTo(0, 0);
      return;
    }
    ResetDelays(BTN_COUNT, (uint32_t *)&general.control.btnTimePressed);
  }
  //}
}

void NavigateShared(void) {
  if (IS_ONLY_PRESSED(2)) {
    if (general.control.btnTimePressed[2] >= 1000) {
      if (blockBetweenScr == 1)
        return;
      blockBetweenScr = 0;
      // Принудительное включение, если нет ошибок
      if (((general.state.enableDelayFlags & ENABLE_RELAY_DELAY) == 0 && general.screens.mainScrValues.crashes[0] != 0) ||
          (general.state.enableDelayFlags & ENABLE_RELAY_DELAY) && general.screens.mainScrValues.errors[0] == 0) {
        general.screens.mainScrValues.isPaused = 0;
        EnableRelayReq();
      } else if (general.screens.mainScrValues.errors[0] == 0 ||
                 (general.screens.mainScrValues.errors[0] == 1 && general.screens.mainScrValues.errors[ManualPowerOffError])) {
        general.screens.mainScrValues.isPaused ^= 1;
        general.screens.mainScrValues.errorChanged = 1;
        if (general.screens.mainScrValues.isPaused)
          DisableRelayReq();
        else
          EnableRelayReq();
      } else if (general.screens.mainScrValues.errors[0] != 0 && general.control.relayState == 0) {
        general.screens.mainScrValues.isPaused ^= 1;
        if ((general.state.enableDelayFlags & ENABLE_RELAY_DELAY))
          general.state.enableDelayFlags &= ~ENABLE_RELAY_DELAY;
        else
          general.state.enableDelayFlags |= ENABLE_RELAY_DELAY;
        general.screens.mainScrValues.errorChanged = 1;
      }
      changeScreenTo(0, 0);
    }
  }
}