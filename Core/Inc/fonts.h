#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>
#include <stddef.h>
#include "ExtraMemories.h"

typedef struct {
  //Start character code
  uint16_t startCharCode;
  //End character code
  uint16_t endCharCode;
  uint16_t *charDescriptoprs;
} LookupBlock;

typedef struct {
  //Character height in bytes
  uint8_t charHeight;
  //Start character code
  uint16_t startCharCode;
  //End character code
  uint16_t endCharCode;
  //Space width in bits
  uint8_t spaceCharWidth;
  //Character descriptors in blocks
  uint8_t blocksCount;
  LookupBlock *blocks;
  //Character descriptors in one block
  LookupBlock charBlock;
  //Character bitmaps
  uint8_t *charBitmaps;
} FontInfo;

extern FontInfo bahnschrift_14ptFontInfo;
extern FontInfo arialBlack_8ptFontInfo;
extern FontInfo arialBlack_10ptFontInfo;
extern FontInfo arialBlack_12ptFontInfo;
extern FontInfo arialBlack_16ptFontInfo;
extern FontInfo arialBlack_20ptFontInfo;
extern FontInfo arialBlack_24ptFontInfo;

#endif //FONTS_H
