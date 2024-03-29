#ifndef _EE24CONFIG_H
#define _EE24CONFIG_H

#include "main.h"

#define _EEPROM_SIZE_KBIT 8
#define _EEPROM_I2C EEPROM_I2C_Handle
#define _EEPROM_USE_FREERTOS 1
#define _EEPROM_ADDRESS 0xA0
#define _EEPROM_USE_WP_PIN 0

#if (_EEPROM_USE_WP_PIN == 1)
#define _EEPROM_WP_GPIO EE_WP_GPIO_Port
#define _EEPROM_WP_PIN EE_WP_Pin
#endif

#endif
