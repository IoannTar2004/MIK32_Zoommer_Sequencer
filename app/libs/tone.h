#ifndef TONE
#define TONE

#include "timer32.h"
#include "utils/pins.h"

void tone_init(uint8_t pin_num, TIMER32_TypeDef* _pwm_timer, TIMER32_TypeDef* _duration_timer);
void tone(float freq, float duration);
void set_position_change(uint8_t* _position);

#endif