#include "libs/ssd1306.h"
#include "xprintf.h"
#include <string.h>


static void print_A(uint8_t x, uint8_t y);
static void print_B(uint8_t x, uint8_t y);
static void print_C(uint8_t x, uint8_t y);
static void print_D(uint8_t x, uint8_t y);
static void print_E(uint8_t x, uint8_t y);
static void print_F(uint8_t x, uint8_t y);
static void print_G(uint8_t x, uint8_t y);

static void print_2(uint8_t x, uint8_t y);
static void print_3(uint8_t x, uint8_t y);
static void print_4(uint8_t x, uint8_t y);
static void print_5(uint8_t x, uint8_t y);
static void print_6(uint8_t x, uint8_t y);

static void print_sharp(uint8_t x, uint8_t y);

static void (*print_letters[])(uint8_t, uint8_t) = {
    print_A, print_B, print_C, print_D, print_E, print_F, print_G
};

static void (*print_numbers[])(uint8_t, uint8_t) = {
    print_2, print_3, print_4, print_5, print_6
};

void print_note(uint8_t x, uint8_t y, char* note) {    
    int size = strlen(note);
    if (size > 3) return;

    for (int i = 0; i < size; i++) {
        if (note[i] >= 'A' && note[i] <= 'G')
            print_letters[note[i] - 'A'](x + i * 11, y);
        else if (note[i] >= '2' && note[i] <= '6')
            print_numbers[note[i] - '2'](x + i * 11, y);
        else
            print_sharp(x + i * 11, y);
    }
    
}

void print_A(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x + 2, y, 5, 2);
    oled_draw_rectangle(x, y + 2, 2, 10);
    oled_draw_rectangle(x + 7, y + 2, 2, 10);
    oled_draw_rectangle(x + 2, y + 5, 5, 2);
}

void print_B(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, 7, 2);
    oled_draw_rectangle(x, y + 2, 2, 8);
    oled_draw_rectangle(x, y + 10, 7, 2);
    oled_draw_rectangle(x + 7, y + 2, 2, 8);
    oled_draw_rectangle(x + 2, y + 5, 5, 2);
}

void print_C(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, 9, 2);
    oled_draw_rectangle(x, y + 2, 2, 8);
    oled_draw_rectangle(x, y + 10, 9, 2);
}

void print_D(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, 7, 2);
    oled_draw_rectangle(x, y + 2, 2, 8);
    oled_draw_rectangle(x, y + 10, 7, 2);
    oled_draw_rectangle(x + 8, y + 2, 2, 8);
}

void print_E(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, 9, 2);
    oled_draw_rectangle(x, y + 2, 2, 8);
    oled_draw_rectangle(x, y + 10, 9, 2);
    oled_draw_rectangle(x + 2, y + 5, 7, 2);
}

void print_F(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, 9, 2);
    oled_draw_rectangle(x, y + 2, 2, 10);
    oled_draw_rectangle(x + 2, y + 5, 7, 2);
}

void print_G(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x + 1, y, 7, 2);
    oled_draw_rectangle(x, y + 1, 2, 10);
    oled_draw_rectangle(x + 1, y + 10, 7, 2);
    oled_draw_rectangle(x + 7, y + 5, 2, 6);
    oled_draw_rectangle(x + 4, y + 5, 3, 2);
}

void print_2(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, 8, 2);
    oled_draw_rectangle(x + 6, y + 2, 2, 5);
    oled_draw_rectangle(x, y + 5, 6, 2);
    oled_draw_rectangle(x, y + 7, 2, 5);
    oled_draw_rectangle(x + 2, y + 10, 6, 2);
}

void print_3(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, 8, 2);
    oled_draw_rectangle(x + 6, y + 2, 2, 10);
    oled_draw_rectangle(x, y + 5, 6, 2);
    oled_draw_rectangle(x, y + 10, 6, 2);
}

void print_4(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, 2, 7);
    oled_draw_rectangle(x + 6, y, 2, 12);
    oled_draw_rectangle(x + 2, y + 5, 4, 2);
}

void print_5(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, 8, 2);
    oled_draw_rectangle(x, y + 2, 2, 5);
    oled_draw_rectangle(x + 2, y + 5, 6, 2);
    oled_draw_rectangle(x + 6, y + 7, 2, 5);
    oled_draw_rectangle(x, y + 10, 6, 2);
}

void print_6(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, 8, 2);
    oled_draw_rectangle(x, y + 2, 2, 8);
    oled_draw_rectangle(x + 2, y + 5, 6, 2);
    oled_draw_rectangle(x + 6, y + 7, 2, 5);
    oled_draw_rectangle(x, y + 10, 6, 2);
}

void print_sharp(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x + 2, y, 1, 12);
    oled_draw_rectangle(x + 6, y, 1, 12);
    oled_draw_rectangle(x, y + 3, 9, 1);
    oled_draw_rectangle(x, y + 9, 9, 1);
}