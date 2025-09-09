#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdint.h>

class Environment
{
  protected:
    uint8_t w, h;

  public:
    Environment(uint8_t width, uint8_t height);

    virtual void init() = 0;
    virtual void set_pos(uint8_t x, uint8_t y) = 0;
    virtual void send_data(uint8_t data) = 0;
    virtual void send_pack(uint8_t data, uint8_t times) = 0;
    virtual void send_mult(uint8_t* data, uint8_t size) = 0;
    virtual void clear() = 0;
};

#endif
