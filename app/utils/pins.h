#ifndef PINS
#define PINS

#include "mik32_hal_gpio.h"

#define LOW 0
#define HIGH 1

void pin_mode(uint8_t pin_num, HAL_GPIO_ModeTypeDef pin_mode);
void digital_write(uint8_t pin_num, uint8_t level);
uint32_t digital_read(uint8_t pin_num);

#endif