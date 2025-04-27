#include <inttypes.h>
#include "project/display.h"
#include "libs/ssd1306.h"
#include "project/IRcontroller.h"
#include "libs/IRreciever.h"
#include "xprintf.h"

#define SEQUENCER 0
#define BPM 1

static uint8_t bpm = 120;
static uint8_t page = SEQUENCER;
static int8_t cursor = 0;

static void print_cursor(uint8_t mode) {
    oled_set_print_mode(mode);

    uint8_t x = cursor % 4, y = cursor / 4;
    oled_draw_rectangle(32 * x, 16 * y, 32, 1);
    oled_draw_rectangle(32 * x, 16 * y, 1, 16);
    oled_draw_rectangle(32 * x + 31, 16 * y, 1, 16);
}

void open_sequencer_page() {
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            oled_draw_rectangle(2 + j * 32, 15 + i * 16, 28, 1);
        }
    }
    print_cursor(PRINT);
}

void ir_change_page() {
    oled_clear_screen();
    if (page == SEQUENCER)
        print_bpm(bpm);
    else
        open_sequencer_page();
    
    page = !page;
}

void ir_press_plus_minus(uint8_t plus) {
    if (page == BPM) {
        if (plus && bpm == 255 || !plus && bpm == 60)
            return;
        oled_clear_screen();
        bpm = plus ? bpm + 1: bpm - 1;
        print_bpm(bpm);
    } else {
        // print_cursor(CLEAR);
        // cursor = plus ? cursor + 1 : cursor - 1;
        // if (cursor == 16)
        //     cursor = 0;
        // else if (cursor == -1)
        //     cursor = 15;
        // print_cursor(PRINT);
    }
}

void ir_press_next_prev(uint8_t next) {
    if (page != SEQUENCER)
        return;

    print_cursor(CLEAR);
    cursor = next ? cursor + 1 : cursor - 1;
    if (cursor == 16)
        cursor = 0;
    else if (cursor == -1)
        cursor = 15;
    print_cursor(PRINT);
}

void ir_press_up_down(uint8_t up) {
    if (page != SEQUENCER)
        return;
    
    print_cursor(CLEAR);
    cursor = up ? cursor + 4 : cursor - 4;
    if (cursor > 15)
        cursor %= 4;
    else if (cursor < 0)
        cursor += 16;
    print_cursor(PRINT);
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

#define set_edge_bpm(value) \
    oled_clear_screen();    \
    print_bpm(value);         \
    bpm = value;

static void ir_set_bpm(uint32_t code) {
    uint8_t number = get_number_from_ircode(code);
    if (number == 0)
        return;
    oled_set_print_mode(CLEAR);
    // print

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
    if (new_bpm > 25) {
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
    } else if (new_bpm < 100) {
        oled_clear_screen();
        print_bpm(new_bpm);
    }
    bpm = new_bpm;

}

static void clear_current_note() {

}

static void ir_set_note(uint32_t code) {
//     uint8_t number = get_number_from_ircode(code);
//     if (number == 0)
//         return;
//     oled_clear_screen();

//     uint8_t start = 10, step = 41;
//     PrintFunc* print_big_numbers = get_big_numbers_array();

//     print_big_numbers[number](start, 5);
//     uint16_t new_bpm = number;
//     code = 0;
//     for (volatile int i = 0; i < 25000; i++)
//         if (ir_decode(&code)) break;

//     new_bpm *= 10;
//     if (code == 0) {
//         new_bpm *= 10;
//         goto end;
//     }

//     number = get_number_from_ircode(code); 
//     new_bpm += number;
//     print_big_numbers[number](start + step, 5);
//     if (new_bpm > 25) {
//         goto end;
//     }
//     code = 0;
//     for (volatile int i = 0; i < 25000; i++)
//         if (ir_decode(&code)) break;
    
//     new_bpm *= 10;
//     if (code == 0) {
//         print_big_numbers[0](start + 2 * step, 5);
//     } else {
//         number = get_number_from_ircode(code); 
//         new_bpm += number;
//         print_big_numbers[number](start + 2 * step, 5);
//     }
    
// end:
//     if (new_bpm > 255) {
//         set_edge_bpm(255);
//     } else if (new_bpm < 60) {
//         set_edge_bpm(60);
//     } else if (new_bpm < 100) {
//         oled_clear_screen();
//         print_bpm(new_bpm);
//     }
//     bpm = new_bpm;
}

void ir_press_numbers_init(uint32_t code) {
    if (page == BPM)
        ir_set_bpm(code);
    // else
}