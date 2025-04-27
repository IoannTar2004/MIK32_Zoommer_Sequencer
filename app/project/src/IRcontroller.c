#include <inttypes.h>
#include "libs/IRreciever.h"
#include "project/sequencer_logic.h"
#include "project/IRcontroller.h"

void contoller_init(uint8_t pin) {
    ir_set_pin(pin);
}

inline static void check_if_number_press(uint32_t code) {
    static uint32_t number_codes[] = {
        BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4,
        BUTTON_5, BUTTON_6, BUTTON_7, BUTTON_8, BUTTON_9
    };
    for (int i = 0; i < sizeof(number_codes) / sizeof(uint32_t); i++){
        if (code == number_codes[i])
            ir_press_numbers_init(number_codes[i]);
    }
    
}

void controller_decode() {
    uint32_t code;
    if (!ir_decode(&code))
        return;

    check_if_number_press(code);
    switch (code) {
        case BUTTON_PLUS:
            ir_press_inc_dec(1);
            break;
        case BUTTON_MINUS:
            ir_press_inc_dec(0);
            break;
        default:
            break;
    }
    code = 0;
}