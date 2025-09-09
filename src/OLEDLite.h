#ifndef OLEDLITE_H
#define OLEDLITE_H

#include <Arduino.h>
#include <Wire.h>

class Display
{
  private:
    uint8_t adr, w, h;
    static const uint8_t I2C_BUFFER_LIMIT = 30;

    void send_command(uint8_t);
    void send_command(uint8_t, uint8_t);

  public:
    Display(uint8_t, uint8_t, uint8_t);
    void init();
    bool set_pos(uint8_t, uint8_t);
    void send_data(uint8_t);
    bool send_pack(uint8_t, uint8_t);
    bool send_mult(uint8_t*, uint8_t);
    void clear();
};

#endif
