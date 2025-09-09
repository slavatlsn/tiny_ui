#ifndef WIDGETS_H
#define WIDGETS_H

#include "Environment.h"
#include "Font.h"
#include <avr/pgmspace.h>
#include <Arduino.h>

//=======================================
// База для графического компонента
//=======================================

class Widget
{
  protected:
    Environment* env;
    uint8_t x1, x2;
    uint8_t y1, y2;

  public:
    Widget(Environment& environment, uint8_t x_1, uint8_t x_2, uint8_t y_1, uint8_t y_2);

    virtual void render() = 0;
    void clear();
};

//=======================================
// компонент ProgressBar
//=======================================

class ProgressBar : public Widget
{
private:
    uint8_t len, prog, prim;
    bool rend, init, border;

public:
    ProgressBar(Environment& environment, uint8_t page, uint8_t start, uint8_t end, uint8_t progress = 0);

    void style(bool s);
    void set(uint8_t progress);
    uint8_t get();
    void render() override;
};

//=======================================
// компонент Toggle
//=======================================

class Toggle : public Widget
{
private:
    uint8_t len;
    bool st, rend, init, border, inner;

    void buf(uint8_t len, uint8_t inn);

public:
    Toggle(Environment& environment, uint8_t page, uint8_t start, uint8_t end, bool state = false);

    void set(bool state);
    uint8_t get();
    void style(bool s, bool s2);
    void render() override;
};

//=======================================
// компонент Text
//=======================================

class Text : public Widget
{
  private:
    uint8_t len, h;
    String txt;

    bool draw_char(uint8_t& pos, uint8_t& page, char c);

  public:
    Text(Environment& environment,
      uint8_t page_start, uint8_t page_end,
      uint8_t start, uint8_t end,
      String text = String(""));

    void set(String text);
    void render() override;
};

#endif
