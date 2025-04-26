#ifndef PINS
#define PINS

#include "mik32_hal_gpio.h"

#define LOW 0
#define HIGH 1

typedef struct pins {
    GPIO_TypeDef* gpio;
    uint32_t pin_num;
    int8_t pwm_channel;
} pins;

pins get_pin(uint8_t n);
void pin_mode(uint8_t pin_num, HAL_GPIO_ModeTypeDef pin_mode);
void pin_mode_init(uint8_t pin_num, uint8_t mode, uint8_t pull, uint8_t ds);
void digital_write(uint8_t pin_num, uint8_t level);
uint32_t digital_read(uint8_t pin_num);

#endif