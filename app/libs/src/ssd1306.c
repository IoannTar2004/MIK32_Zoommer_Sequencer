#include "mik32_hal_spi.h"
#include "utils/pins.h"
#include "utils/delays.h"
#include "xprintf.h"

static SPI_HandleTypeDef spi;
static uint8_t dc;
uint8_t rx[128]; // буфер для получения данных по spi (которых нет)
uint8_t display_buffer[8][128];

void oled_init(SPI_HandleTypeDef _spi, uint8_t rst_pin, uint8_t dc_pin) {
    spi = _spi;
    dc = dc_pin;

    pin_mode(rst_pin, __OUTPUT);
    pin_mode(dc, __OUTPUT);
    digital_write(rst_pin, LOW);
    delay(10);
    digital_write(rst_pin, HIGH);
    delay(10);

    digital_write(dc, LOW);
    uint8_t init[] = {0xC8, 0xA1, 0x8D, 0x14, 0xAF};
    HAL_SPI_Exchange(&spi, init, rx, sizeof(init), -1);

    uint8_t black[128] = { [0 ... 127] = 0x0 };
    for (int page = 0; page < 8; page++) {
        uint8_t set_page[] = {0xB0 + page, 0x00, 0x10};
        digital_write(dc, LOW);
        HAL_SPI_Exchange(&spi, set_page, rx, 3, -1);

        digital_write(dc, HIGH);
        HAL_SPI_Exchange(&spi, black, rx, sizeof(black), -1);
    }
}

static void add_to_display_buffer(uint8_t page, uint8_t x, uint8_t length, uint8_t pixel) {
    for (int i = x; i < x + length; i++)
        display_buffer[page][i] |= pixel;
    
    uint8_t set_page[] = {0xB0 + page, x & 0xF, x >> 4 | 0x10};
    digital_write(dc, LOW);
    HAL_SPI_Exchange(&spi, set_page, rx, 3, -1);

    digital_write(dc, HIGH);
    HAL_SPI_Exchange(&spi, &display_buffer[page][x], rx, length, -1);
}

void oled_draw_rectangle(uint8_t x, uint8_t y, uint8_t length, int8_t width) {
    length = length > 128 - x ? 128 - x : length;
    width = width > 64 - y ? 64 - y : width;
    int page = y / 8;

    uint8_t h_nibble = width + y % 8 > 8 ? 0xFF : ~(-1 << (width + y % 8));
    add_to_display_buffer(page, x, length, (uint8_t)(-1 << y % 8) & h_nibble);

    width -= 8 - (y % 8);
    y += 8 - (y % 8);
    page++;
    for (; width > 8; page++) {
        add_to_display_buffer(page, x, length, 0xFF);
        width -= 8;
    }  
    
    if (width > 0) {
        h_nibble = ~(-1 << (width + y % 8));
        add_to_display_buffer(page, x, length, (uint8_t)(-1 << y % 8) & h_nibble);
    }
}

