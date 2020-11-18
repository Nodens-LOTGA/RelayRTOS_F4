#ifndef _EE24_H
#define _EE24_H

#include "ee24Config.h"
#include <stdbool.h>
#include <stdint.h>

bool ee24_isConnected(void);
bool ee24_write(uint16_t address, uint8_t *data, unsigned int lenInBytes, uint32_t timeout);
bool ee24_read(uint16_t address, uint8_t *data, unsigned int lenInBytes, uint32_t timeout);
bool ee24_eraseChip(void);

#endif
