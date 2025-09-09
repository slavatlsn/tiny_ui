#include "OLEDLite.h"

Display::Display(uint8_t address, uint8_t width, uint8_t height) : adr(address), w(width), h(height/8) {}

void Display::send_command(uint8_t cmd)
{
  Wire.beginTransmission(adr);
  Wire.write(0x00);
  Wire.write(cmd);
  Wire.endTransmission();
}

void Display::send_command(uint8_t cmd, uint8_t arg)
{
  Wire.beginTransmission(adr);
  Wire.write(0x00);
  Wire.write(cmd);
  Wire.write(arg);
  Wire.endTransmission();
}

void Display::init()
{
  Wire.begin();
  send_command(0xAE);
  send_command(0xD5, 0x80);
  send_command(0xA8, 0x3F);
  send_command(0xD3, 0x00);
  send_command(0x40);
  send_command(0x8D, 0x14);
  send_command(0x20, 0x00);
  send_command(0xA1);
  send_command(0xC8);
  send_command(0xDA, 0x12);
  send_command(0x81, 0x7F);
  send_command(0xA4);
  send_command(0xA6);
  send_command(0xAF);
}

bool Display::set_pos(uint8_t x, uint8_t y)
{
  if (x >= w || y >= h) return false;
  Wire.beginTransmission(adr);
  Wire.write(0x00);
  Wire.write(0xB0 | y);
  Wire.write(0x21);
  Wire.write(x);
  Wire.write(0x7F);
  Wire.endTransmission();
  return true;
}

void Display::send_data(uint8_t data)
{
  Wire.beginTransmission(adr);
  Wire.write(0x40);
  Wire.write(data);
  Wire.endTransmission();
}

bool Display::send_pack(uint8_t data, uint8_t times)
{
  if (times == 0) return false;
  uint8_t i = 0;
  while (i < times)
  {
    if (i % I2C_BUFFER_LIMIT == 0)
    {
      if (i != 0) Wire.endTransmission();
      Wire.beginTransmission(adr);
      Wire.write(0x40);
    }
    Wire.write(data);
    i++;
  }
  Wire.endTransmission();
  return true;
}

bool Display::send_mult(uint8_t* data, uint8_t size)
{
  if (size == 0 || data == nullptr) return false;
  uint8_t i = 0;
  while (i < size)
  {
    if (i % I2C_BUFFER_LIMIT == 0)
    {
      if (i != 0) Wire.endTransmission();
      Wire.beginTransmission(adr);
      Wire.write(0x40);
    }
    Wire.write(data[i]);
    i++;
  }
  Wire.endTransmission();
  return true;
}

void Display::clear()
{
  for (uint8_t page = 0; page < h; page++)
  {
    Wire.beginTransmission(adr);
    Wire.write(0x00);
    Wire.write(0xB0 | page);
    Wire.write(0x21);
    Wire.write(0x00);
    Wire.write(0x7F);
    Wire.endTransmission();
    send_pack(0x00, 128);
  }
}
