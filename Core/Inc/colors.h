#ifndef COLORS_H
#define COLORS_H

#include "display.h"

extern Color blackColor;
extern Color greenColor;
extern Color blueColor;
extern Color purpleColor;
extern Color redColor;
extern Color whiteColor;
extern Color cyanColor;
extern Color yellowColor;
extern Color orangeColor;
extern Color tealColor;

typedef enum { BgCyanColor, BgTealColor, BgOrangeColor, BgYellowColor, BgWhiteColor, BgColorCount } BgColors;

extern const Color *bgColors[];

#endif