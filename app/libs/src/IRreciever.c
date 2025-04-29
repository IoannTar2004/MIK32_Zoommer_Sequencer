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
            if (digital_read(pin))
                return 0;
            for (int i = 0; i < 32; i++) {
                while (!digital_read(pin));
                delayMicroseconds(700);

                *data <<= 1;
                if (digital_read(pin)) {
                    *data |= 1;
                    while (digital_read(pin));
                }
            }
            return 1;
        }
    }
    return 0;
}
