#include <inttypes.h>
#include "project/display.h"
#include "libs/ssd1306.h"
#include "project/IRcontroller.h"
#include "libs/IRreciever.h"
#include "xprintf.h"

#define SEQUNCER 0
#define BPM 1

static uint8_t bpm = 120;
static uint8_t page = BPM;

void sequencer_init() {
    print_bpm(bpm);
}

void ir_press_inc_dec(uint8_t inc) {
    if (inc && bpm == 255 || !inc && bpm == 60)
        return;
    if (page == BPM) {
        oled_set_print_mode(CLEAR);
        oled_draw_rectangle(0, 0, 128, 64);
        oled_set_print_mode(PRINT);
        bpm = inc ? bpm + 1: bpm - 1;
        print_bpm(bpm);
    }
}

static uint8_t get_number_from_ircode(uint32_t code) {
    switch (code) {
        case BUTTON_0: return 0;
        case BUTTON_1: return 1;
        case BUTTON_2: return 2;
        case BUTTON_3: return 3;
        case BUTTON_4: return 4;
        case BUTTON_5: return 5;
        case BUTTON_6: return 6;
        case BUTTON_7: return 7;
        case BUTTON_8: return 8;
        case BUTTON_9: return 9;
    }
}

static void ir_set_bpm() {

}

#define set_edge_bpm(value) \
    oled_clear_screen();    \
    print_bpm(value);         \
    bpm = value;

void ir_press_numbers_init(uint32_t code) {
    if (page == BPM) {
        uint8_t number = get_number_from_ircode(code);
        if (number == 0)
            return;
        oled_clear_screen();

        uint8_t start = 10, step = 41;
        PrintFunc* print_big_numbers = get_big_numbers_array();

        print_big_numbers[number](start, 5);
        uint16_t new_bpm = number;
        code = 0;
        for (volatile int i = 0; i < 25000; i++)
            if (ir_decode(&code)) break;

        new_bpm *= 10;
        if (code == 0) {
            new_bpm *= 10;
            goto end;
        }

        number = get_number_from_ircode(code); 
        new_bpm += number;
        print_big_numbers[number](start + step, 5);
        if (new_bpm < 100 && new_bpm > 25) {
            goto end;
        }
        code = 0;
        for (volatile int i = 0; i < 25000; i++)
            if (ir_decode(&code)) break;
        
        new_bpm *= 10;
        if (code == 0) {
            print_big_numbers[0](start + 2 * step, 5);
        } else {
            number = get_number_from_ircode(code); 
            new_bpm += number;
            print_big_numbers[number](start + 2 * step, 5);
        }
        
    end:
        if (new_bpm > 255) {
            set_edge_bpm(255);
        } else if (new_bpm < 60) {
            set_edge_bpm(60);
        }
        else {
            print_bpm(new_bpm);
            bpm = new_bpm;
        }
            
    }
}