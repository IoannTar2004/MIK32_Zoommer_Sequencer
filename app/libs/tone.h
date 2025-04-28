#ifndef TONE
#define TONE

#include "timer32.h"
#include "utils/pins.h"

void tone_init(uint8_t pin_num, TIMER32_TypeDef* _pwm_timer, TIMER32_TypeDef* _duration_timer);
void tone(uint32_t freq, float duration);
inline uint8_t get_stop_flag();
inline void reset_stop_flag();

#endif