#ifndef UTILS_H
#define UTILS_H
#include "main.h"

uint8_t GetMaxDay(uint8_t month, uint8_t year);

uint32_t CountRmsValueX100(int16_t *values, int16_t size, int16_t zeroPoint);
uint32_t math_sqrt(register uint32_t value);
int compare_uint16(const void *a, const void *b);
int sgn(int32_t val);
int abs(int v);
void fft_magnitude(uint32_t *values, uint16_t len);
void calculateTimerReg(uint32_t wantedDelayUs, TIM_TypeDef *timer, volatile uint32_t *divisor, volatile uint32_t *timerPeriod);
#endif //UTILS_H