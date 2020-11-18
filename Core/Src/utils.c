#include "utils.h"

#define C_REAL(X) (X & 0xFFFF)
#define C_IMAG(X) (X >> 16)

#define IS_TIMER_2_3_4_2xPLL_ENABLED() (0 < (((RCC->CFGR & RCC_CFGR_PPRE1) >> POSITION_VAL(RCC_CFGR_PPRE1)) & 4))
#define IS_TIMER_1_2xPLL_ENABLED() (0 < (((RCC->CFGR & RCC_CFGR_PPRE2) >> POSITION_VAL(RCC_CFGR_PPRE2)) & 4))

#define TIMER_2_3_4_FREQ() (IS_TIMER_2_3_4_2xPLL_ENABLED() ? (HAL_RCC_GetPCLK1Freq() << 1) : (HAL_RCC_GetPCLK1Freq()))
#define TIMER_1_FREQ() (IS_TIMER_1_2xPLL_ENABLED() ? (HAL_RCC_GetPCLK2Freq() << 1) : (HAL_RCC_GetPCLK2Freq()))

#define GET_TIMER_FREQ(INSTANCE) (INSTANCE == TIM1 ? TIMER_1_FREQ() : TIMER_2_3_4_FREQ())

static uint8_t IsLeapYear(uint16_t nYear) {
  if ((nYear % 4U) != 0U)
    return 0U;

  if ((nYear % 100U) != 0U)
    return 1U;

  if ((nYear % 400U) == 0U)
    return 1U;
  else
    return 0U;
}

uint8_t GetMaxDay(uint8_t month, uint8_t year) {
  if ((month == 1U) || (month == 3U) || (month == 5U) || (month == 7U) || (month == 8U) || (month == 10U) || (month == 12U))
    return 31U;
  else if ((month == 4U) || (month == 6U) || (month == 9U) || (month == 11U))
    return 30U;
  else if (month == 2U) {
    /* Leap year */
    if (IsLeapYear(year))
      return 29U;
    else
      return 28U;
  }
  return 0;
}

uint32_t CountRmsValueX100(int16_t *values, int16_t size, int16_t zeroPoint) {
  uint64_t buffer = 0;
  int32_t tmp = 0;
  for (int i = 0; i < size; i++) {
    tmp = values[i] - zeroPoint;
    tmp = tmp * tmp;
    buffer += (uint64_t)(tmp);
  }
  buffer *= 100;
  buffer /= size;
  return math_sqrt((uint32_t)buffer);
}

uint32_t math_sqrt(register uint32_t value) {
  register uint32_t d = 0, ax = 0;
  __asm volatile("CLZ    %[A], %[VALUE]"
                 "\n\t"
                 "RSB    %[A], %[A], 0x1F"
                 : [A] "=&r"(ax)
                 : [VALUE] "r"(value));
  d = value >> (ax >> 1);
  ax = (d + value / d) >> 1;
  d = (ax + value / ax) >> 1;
  ax = (d + value / d) >> 1;
  value = (ax + value / ax) >> 1;
  return value;
}

int compare_uint16(const void *a, const void *b) {
  const uint16_t ai = *(const uint16_t *)a;
  const uint16_t bi = *(const uint16_t *)b;

  if (ai < bi) {
    return -1;
  } else if (ai > bi) {
    return 1;
  } else {
    return 0;
  }
}

int sgn(int32_t val) { return (0 < val) - (val < 0); }

int abs(int v) { return v * ((v > 0) - (v < 0)); }

void fft_magnitude(uint32_t *values, uint16_t len) {
  for (uint16_t i = 0; i < len; i++) {
    int16_t rl = C_REAL(values[i]);
    int16_t im = C_IMAG(values[i]);
    uint32_t mag = math_sqrt(rl * rl + im * im);
    values[i] = mag;
  }
}

void calculateTimerReg(uint32_t wantedDelayUs, TIM_TypeDef* timer, volatile uint32_t *divisor, volatile uint32_t *timerPeriod) {
  uint32_t clockFreqBeforeDivision;
  float perfectClockFreqAfterDivision, actualClockFreqAfterDivision;
  clockFreqBeforeDivision = GET_TIMER_FREQ(timer);
  perfectClockFreqAfterDivision = (((uint64_t)(1000000) * 65536) / ((uint64_t)wantedDelayUs));
  *divisor = (uint32_t)(1 + (clockFreqBeforeDivision / perfectClockFreqAfterDivision));
  actualClockFreqAfterDivision = clockFreqBeforeDivision / ((uint32_t)(*divisor));
  (*divisor)--;
  *timerPeriod = (uint32_t)wantedDelayUs * (actualClockFreqAfterDivision / 1000000) - 1;
}