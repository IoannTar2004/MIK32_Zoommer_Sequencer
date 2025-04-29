#include <inttypes.h>
#include "project/display.h"
#include "project/IRcontroller.h"
#include "project/sequencer_logic.h"
#include "libs/ssd1306.h"
#include "libs/IRreciever.h"

#define SEQUENCER 0
#define BPM 1

static volatile uint8_t bpm = 120;
static volatile uint8_t page = SEQUENCER;
static volatile int8_t cursor = 0;
static volatile uint8_t play = 0;
static volatile uint8_t last_note = 255;

static char* notes_array[] = {
    "C", "C#", "D", "D#", "E", "F", 
    "F#", "G", "G#", "A", "A#", "B"
};
uint16_t note_frequencies[] = {
    65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117, 123,
    131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247,
    262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
    523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988
};

static volatile sequence sequencer[16];

static void clear_current_note();
static void create_note(char* note, uint8_t note_number, uint8_t pos);
static void print_note_to_cell(uint8_t note_number, uint8_t pos);
static void open_sequencer_page();
static void update_note_freq(uint8_t pos);

void sequncer_init() {
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++) {
            uint8_t index = j + i * 4;
            sequencer[index].note = -1;
            sequencer[index].freq = 0;
            sequencer[index].duration = 125;
        }
    }
    open_sequencer_page();
}

static void print_cursor(uint8_t mode) {
    oled_set_print_mode(mode);

    uint8_t x = cursor % 4, y = cursor / 4;
    oled_draw_rectangle(32 * x, 16 * y, 32, 1);
    oled_draw_rectangle(32 * x, 16 * y, 1, 16);
    oled_draw_rectangle(32 * x + 31, 16 * y, 1, 16);
}

void open_sequencer_page() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            oled_draw_rectangle(2 + j * 32, 15 + i * 16, 28, 1);
            print_note_to_cell(sequencer[j + i * 4].note, j + i * 4);
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

static void print_note_to_cell(uint8_t note_number, uint8_t pos) {
    sequencer[pos].note = note_number;
    last_note = note_number;
    update_note_freq(pos);

    if (note_number > 48) {
        clear_current_note();
        return;
    }

    char note[4];
    create_note(note, note_number, pos);
    uint8_t x = note[2] == '\0' ? 7 : 2;
    print_note(x + (pos % 4) * 32, 2 + (pos / 4) * 16, note);
}

inline void update_note_freq(uint8_t pos) {
    sequencer[pos].freq = sequencer[pos].note < 48 ? note_frequencies[sequencer[pos].note] : 0;
}

static inline void update_note_duration() {
    for (size_t i = 0; i < 16; i++) {
        sequencer[i].duration = 15000 / bpm;
    }
    
}

void print_last_note() {
    clear_current_note();
    print_note_to_cell(last_note, cursor);
}

void ir_press_plus_minus(uint8_t plus) {
    if (page == BPM) {
        if ((plus && bpm == 255) || (!plus && bpm == 60))
            return;
        oled_clear_screen();
        bpm = plus ? bpm + 1: bpm - 1;
        print_bpm(bpm);
    } else {
        clear_current_note();
        sequencer[cursor].note = plus ? sequencer[cursor].note + 1 : sequencer[cursor].note - 1;
        if (sequencer[cursor].note > 47 && sequencer[cursor].note != 254)
            sequencer[cursor].note = 255;
        else if (sequencer[cursor].note == 254) {
            sequencer[cursor].note = 47;
        }

        print_note_to_cell(sequencer[cursor].note, cursor);
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

static void clear_current_note() {
    oled_set_print_mode(CLEAR);
    oled_draw_rectangle(2 + (cursor % 4) * 32, 2 + (cursor / 4) * 16, 28, 13);
    oled_set_print_mode(PRINT);
}

static void create_note(char* note, uint8_t note_number, uint8_t pos) {
    sequencer[pos].note = note_number;
    note[0] = notes_array[note_number % 12][0];
    if (notes_array[note_number % 12][1] == '#') {
        note[1] = '#';
        note[2] = note_number / 12 + 2 + '0';
        note[3] = '\0';
    } else {
        note[1] = note_number / 12 + 2 + '0';
        note[2] = '\0';
    }
}

static void ir_set_note(uint32_t code) {
    uint8_t number = get_number_from_ircode(code);

    uint8_t note_number = 0;
    clear_current_note();
    note_number = number;

    code = 0;
    for (volatile int i = 0; i < 25000; i++)
        if (ir_decode(&code)) break;
    
    if (code != 0) {
        number = get_number_from_ircode(code);
        note_number = note_number * 10 + number;
    }
    if (note_number > 47)
        note_number = 255;

    print_note_to_cell(note_number, cursor);
}

#define set_edge_bpm(value) \
    print_bpm(value);         \
    bpm = value;                \
    return;

static void ir_set_bpm(uint32_t code) {
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
    oled_clear_screen();
    if (new_bpm > 255) {
        set_edge_bpm(255);
    } else if (new_bpm < 60) {
        set_edge_bpm(60);
    }
    print_bpm(new_bpm);
    bpm = new_bpm;
    update_note_duration();
}

void ir_press_numbers_init(uint32_t code) {
    if (page == BPM)
        ir_set_bpm(code);
    else
        ir_set_note(code);
}

uint8_t get_bpm() {
    return bpm;
}

void set_bpm(uint8_t _bpm) {
    bpm = _bpm;
}

volatile sequence* get_sequencer() {
    return sequencer;
}

uint8_t get_play() {
    return play;
}

void switch_play() {
    play = !play;
}

void reset_play() {
    play = 0;
}