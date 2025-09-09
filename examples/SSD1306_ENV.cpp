#include "SSD1306_ENV.h"

SSD1306_ENV::SSD1306_ENV(uint8_t width, uint8_t height, uint8_t address)
    : Environment(width, height), screen(address, width, height) {}

void SSD1306_ENV::init()
{
    screen.init();
    screen.clear();
}

void SSD1306_ENV::set_pos(uint8_t x, uint8_t y)
{
    screen.set_pos(x, y);
}

void SSD1306_ENV::send_data(uint8_t data)
{
    screen.send_data(data);
}

void SSD1306_ENV::send_pack(uint8_t data, uint8_t times)
{
    screen.send_pack(data, times);
}

void SSD1306_ENV::send_mult(uint8_t* data, uint8_t size)
{
    screen.send_mult(data, size);
}

void SSD1306_ENV::clear()
{
    screen.clear();
}
