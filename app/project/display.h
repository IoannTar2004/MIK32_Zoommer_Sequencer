#ifndef DISPLAY
#define DISPLAY

typedef void (*PrintFunc)(uint8_t, uint8_t);

void print_note(uint8_t x, uint8_t y, char* note);

void print_big_0(uint8_t x, uint8_t y);
void print_big_1(uint8_t x, uint8_t y);
void print_big_2(uint8_t x, uint8_t y);
void print_big_3(uint8_t x, uint8_t y);
void print_big_4(uint8_t x, uint8_t y);
void print_big_5(uint8_t x, uint8_t y);
void print_big_6(uint8_t x, uint8_t y);
void print_big_7(uint8_t x, uint8_t y);
void print_big_8(uint8_t x, uint8_t y);
void print_big_9(uint8_t x, uint8_t y);

void print_bpm(uint8_t bmp);

PrintFunc* get_big_numbers_array();
PrintFunc* get_letters_array();
PrintFunc* get_numbers_array();

#endif