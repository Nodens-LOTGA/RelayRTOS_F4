#pragma once
#include "stdint.h"

typedef struct {
  uint8_t width;
  uint8_t height;
  const uint32_t *addr;
  const uint8_t count;
} Bitmaps;

extern Bitmaps sv_Bitmaps;