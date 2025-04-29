#include "scr1_timer.h"
#include "mik32_hal.h"
#include "utils/pins.h"
#include "utils/delays.h"
#include "utils/pins.h"
#include "xprintf.h"

static uint8_t pin;

void tone_init(uint8_t pin_num) {
    pin = pin_num;
    pin_mode(pin, __OUTPUT);
}

__attribute__((section(".ram_text"))) 
void tone(uint32_t freq, float ms, void (*callback)(void)) {
    volatile uint64_t end_mtimer = SCR1_TIMER_GET_TIME() + ms * (SYSTEM_FREQ_HZ / 1000);

    while (SCR1_TIMER_GET_TIME() < end_mtimer) {
        if (freq > 60) {
            digital_write(pin, HIGH);
            delayMicroseconds(1000000 / (2 * freq));
            digital_write(pin, LOW);
            delayMicroseconds(1000000 / (2 * freq));
        }
        callback();
    }
}
