#include <inttypes.h>
#include "utils/pins.h"
#include "utils/delays.h"

static uint8_t pin;

void ir_set_pin(uint8_t pin_num) {
    pin = pin_num;
    pin_mode(pin, __INPUT);
}

int ir_decode(uint32_t* data) {
    if (!digital_read(pin)) {
        delayMicroseconds(9200);
        if (digital_read(pin)) {
            delayMicroseconds(4500);
            for (int i = 0; i < 32; i++) {
                while (!digital_read(pin));
                delayMicroseconds(600);

                *data <<= 1;
                if (digital_read(pin)) {
                    *data |= 1; // длинный импульс = 1
                    while (digital_read(pin)); // ждём окончания импульса
                }
            }
            return 1;
        }
    }
    return 0;
}
