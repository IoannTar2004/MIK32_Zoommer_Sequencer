#include <inttypes.h>
#include "mik32_hal_gpio.h"
#include "../pins.h"

static pins gpio_array[] = {
    {.gpio = GPIO_0, .pin_num = GPIO_PIN_5},    // D0
    {.gpio = GPIO_0, .pin_num = GPIO_PIN_6},    // D1
    {.gpio = GPIO_0, .pin_num = GPIO_PIN_10},   // D2
    {.gpio = GPIO_0, .pin_num = GPIO_PIN_0, .pwm_channel = 0},    // D3
    {.gpio = GPIO_0, .pin_num = GPIO_PIN_8},    // D4
    {.gpio = GPIO_0, .pin_num = GPIO_PIN_1, .pwm_channel = 1},    // D5
    {.gpio = GPIO_0, .pin_num = GPIO_PIN_2, .pwm_channel = 2},    // D6
    {.gpio = GPIO_1, .pin_num = GPIO_PIN_8},    // D7
    {.gpio = GPIO_1, .pin_num = GPIO_PIN_9, .pwm_channel = 3},    // D8
    {.gpio = GPIO_0, .pin_num = GPIO_PIN_3},    // D9
    {.gpio = GPIO_1, .pin_num = GPIO_PIN_3, .pwm_channel = 3},    // D10
    {.gpio = GPIO_1, .pin_num = GPIO_PIN_1, .pwm_channel = 1},    // D11
    {.gpio = GPIO_1, .pin_num = GPIO_PIN_0, .pwm_channel = 0},    // D12
    {.gpio = GPIO_1, .pin_num = GPIO_PIN_2, .pwm_channel = 2},    // D13
    {.gpio = GPIO_1, .pin_num = GPIO_PIN_5},    // D14
    {.gpio = GPIO_1, .pin_num = GPIO_PIN_7},    // D15
    {.gpio = GPIO_0, .pin_num = GPIO_PIN_4},    // D16
    {.gpio = GPIO_0, .pin_num = GPIO_PIN_7},    // D17
    {.gpio = GPIO_1, .pin_num = GPIO_PIN_12},    // D18
    {.gpio = GPIO_1, .pin_num = GPIO_PIN_13},    // D19
    {.gpio = GPIO_2, .pin_num = GPIO_PIN_7}     // D20
};

pins get_pin(uint8_t n) {
    return gpio_array[n];
}

void pin_mode(uint8_t pin_num, HAL_GPIO_ModeTypeDef pin_mode) {
    if (pin_num > 20)
        return;

    pins pin = gpio_array[pin_num];
    GPIO_InitTypeDef gpio_settings = {
        .Pin  = pin.pin_num,
        .Mode = pin_mode,
        .Pull = HAL_GPIO_PULL_NONE,
        .DS   = HAL_GPIO_DS_2MA
    };

    HAL_GPIO_Init(pin.gpio, &gpio_settings);
}

void pin_mode_init(uint8_t pin_num, uint8_t mode, uint8_t pull, uint8_t ds) {
    pins pin = gpio_array[pin_num];
    GPIO_InitTypeDef pin_config = {
        .Pin  = pin.pin_num,
        .Mode = mode,
        .Pull = pull,
        .DS   = ds
    };

    HAL_GPIO_Init(pin.gpio, &pin_config);
}

void digital_write(uint8_t pin_num, uint8_t level) {
    pins pin = gpio_array[pin_num];
    if (level)
        pin.gpio->OUTPUT |= pin.pin_num;
    else
        pin.gpio->OUTPUT &= ~pin.pin_num;
}

__attribute__((section(".ram_text")))
uint32_t digital_read(uint8_t pin_num) {
    return (GPIO_0->STATE & (1 << 10)) > 0;
}