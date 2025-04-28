#include <inttypes.h>
#include "libs/IRreciever.h"
#include "project/sequencer_logic.h"
#include "project/IRcontroller.h"
#include "xprintf.h"

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

    // xprintf("0x%x\r\n", code);

    check_if_number_press(code);
    switch (code) {
        case BUTTON_PLUS:
            ir_press_plus_minus(1);
            break;
        case BUTTON_MINUS:
            ir_press_plus_minus(0);
            break;
        case BUTTON_EQ:
            ir_change_page();
            break;
        case BUTTON_CH:
            ir_press_up_down(1);
            break;
        case BUTTON_CH_MINUS:
            ir_press_up_down(0);
            break;
        case BUTTON_NEXT:
            ir_press_next_prev(1);
            break;
        case BUTTON_PREV:
            ir_press_next_prev(0);
            break;
        case BUTTON_PLAY_PAUSE:
            switch_play();
            break;
        case BUTTON_100_PLUS:
            print_last_note();
            break;
    }

    // if (code != BUTTON_PLAY_PAUSE)
    //     reset_play();
    code = 0;
}