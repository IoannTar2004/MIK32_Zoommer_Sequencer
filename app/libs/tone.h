#ifndef TONE
#define TONE

#include "timer32.h"
#include "utils/pins.h"

void tone_init(uint8_t pin_num);
void tone(uint32_t freq, float ms, void (*callback)(void));

#endif