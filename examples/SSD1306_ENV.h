#ifndef SSD1306_ENV_H
#define SSD1306_ENV_H

#include "Environment.h"
#include "OLEDLite.h"

class SSD1306_ENV : public Environment
{
  private:
    Display screen;

  public:
    SSD1306_ENV(uint8_t width, uint8_t height, uint8_t address);

    void init() override;
    void set_pos(uint8_t x, uint8_t y) override;
    void send_data(uint8_t data) override;
    void send_pack(uint8_t data, uint8_t times) override;
    void send_mult(uint8_t* data, uint8_t size) override;
    void clear() override;
};

#endif
