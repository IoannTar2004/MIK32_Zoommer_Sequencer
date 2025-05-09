#ifndef SSD1306
#define SSD1306

#include "mik32_hal_spi.h"

#define PRINT 0
#define CLEAR 1

void oled_init(SPI_HandleTypeDef spi, uint8_t rst_pin, uint8_t dc_pin);
void oled_draw_rectangle(uint8_t x, uint8_t y, uint8_t length, int8_t width);
void oled_clear_screen();
void oled_set_print_mode(uint8_t _mode);

#endif