#include <string.h>
#include "libs/ssd1306.h"
#include "project/display.h"

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

static PrintFunc print_letters[] = {
    print_A, print_B, print_C, print_D, print_E, print_F, print_G
};

static PrintFunc print_numbers[] = {
    print_2, print_3, print_4, print_5, print_6
};

static PrintFunc print_big_numbers[] = {
    print_big_0, print_big_1, print_big_2, print_big_3, print_big_4,
    print_big_5, print_big_6, print_big_7, print_big_8, print_big_9
};

void print_note(uint8_t x, uint8_t y, char* note) {    
    for (int i = 0; note[i] != '\0'; i++) {
        if (note[i] >= 'A' && note[i] <= 'G')
            print_letters[note[i] - 'A'](x + i * 11, y);
        else if (note[i] >= '2' && note[i] <= '6')
            print_numbers[note[i] - '2'](x + i * 10, y);
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
    oled_draw_rectangle(x + 7, y + 2, 2, 8);
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

#define LENGTH 29
#define HEIGHT 55
#define RIGHT LENGTH - THICKNESS
#define HALF_HEIGHT LENGTH
#define THICKNESS 5

void print_big_0(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, LENGTH, THICKNESS);
    oled_draw_rectangle(x + RIGHT, y, THICKNESS, HEIGHT);
    oled_draw_rectangle(x, y + HEIGHT - THICKNESS, LENGTH, THICKNESS);
    oled_draw_rectangle(x, y, THICKNESS, HEIGHT);
}

void print_big_1(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x + RIGHT, y, THICKNESS, HEIGHT);
    oled_draw_rectangle(x + RIGHT - 8, y + 7, 8, THICKNESS);
}

void print_big_2(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, LENGTH, THICKNESS);
    oled_draw_rectangle(x + RIGHT, y, THICKNESS, HALF_HEIGHT);
    oled_draw_rectangle(x, y + RIGHT, LENGTH, THICKNESS);
    oled_draw_rectangle(x, y + RIGHT, THICKNESS, HALF_HEIGHT);
    oled_draw_rectangle(x, y + HEIGHT - THICKNESS, LENGTH, THICKNESS);
}

void print_big_3(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, LENGTH, THICKNESS);
    oled_draw_rectangle(x + RIGHT, y, THICKNESS, HEIGHT);
    oled_draw_rectangle(x, y + RIGHT, LENGTH, THICKNESS);
    oled_draw_rectangle(x, y + HEIGHT - THICKNESS, LENGTH, THICKNESS);
}

void print_big_4(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, THICKNESS, HALF_HEIGHT);
    oled_draw_rectangle(x + RIGHT, y, THICKNESS, HEIGHT);
    oled_draw_rectangle(x, y + RIGHT, LENGTH, THICKNESS);
}

void print_big_5(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, LENGTH, THICKNESS);
    oled_draw_rectangle(x, y, THICKNESS, HALF_HEIGHT);
    oled_draw_rectangle(x, y + RIGHT, LENGTH, THICKNESS);
    oled_draw_rectangle(x + RIGHT, y + RIGHT, THICKNESS, HALF_HEIGHT);
    oled_draw_rectangle(x, y + HEIGHT - THICKNESS, LENGTH, THICKNESS);
}

void print_big_6(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, LENGTH, THICKNESS);
    oled_draw_rectangle(x, y, THICKNESS, HEIGHT);
    oled_draw_rectangle(x, y + RIGHT, LENGTH, THICKNESS);
    oled_draw_rectangle(x + RIGHT, y + RIGHT, THICKNESS, HALF_HEIGHT);
    oled_draw_rectangle(x, y + HEIGHT - THICKNESS, LENGTH, THICKNESS);
}

void print_big_7(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, LENGTH, THICKNESS);
    oled_draw_rectangle(x + RIGHT, y, THICKNESS, HEIGHT);
}

void print_big_8(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, LENGTH, THICKNESS);
    oled_draw_rectangle(x + RIGHT, y, THICKNESS, HEIGHT);
    oled_draw_rectangle(x, y, THICKNESS, HEIGHT);
    oled_draw_rectangle(x, y + RIGHT, LENGTH, THICKNESS);
    oled_draw_rectangle(x, y + HEIGHT - THICKNESS, LENGTH, THICKNESS);
}

void print_big_9(uint8_t x, uint8_t y) {
    oled_draw_rectangle(x, y, LENGTH, THICKNESS);
    oled_draw_rectangle(x + RIGHT, y, THICKNESS, HEIGHT);
    oled_draw_rectangle(x, y, THICKNESS, HALF_HEIGHT);
    oled_draw_rectangle(x, y + RIGHT, LENGTH, THICKNESS);
    oled_draw_rectangle(x, y + HEIGHT - THICKNESS, LENGTH, THICKNESS);
}
    
void print_bpm(uint8_t bpm) {
    uint8_t start = bpm < 100 ? -10 : 10;
    uint8_t step = 41;
    if (bpm >= 100)
        print_big_numbers[bpm / 100](start, 5);
    print_big_numbers[(bpm / 10) % 10](start + step, 5);
    print_big_numbers[bpm % 10](start + 2 * step, 5);
}

PrintFunc* get_big_numbers_array() {
    return print_big_numbers;
}

PrintFunc* get_letters_array() {
    return print_letters;
}

PrintFunc* get_numbers_array() {
    return print_numbers;
}