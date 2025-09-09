#include "Widgets.h"

//=======================================
Widget::Widget(Environment& environment, uint8_t x_1, uint8_t x_2, uint8_t y_1, uint8_t y_2)
  : env(&environment), x1(x_1), x2(x_2), y1(y_1), y2(y_2) {}

void Widget::clear() {
  for (uint8_t i = y1; i <= y2; i++) {
    env->set_pos(x1, i);
    env->send_pack(0, x2 - x1 + 1);
  }
}

//=======================================
ProgressBar::ProgressBar(Environment& environment, uint8_t page, uint8_t start, uint8_t end, uint8_t progress)
  : Widget(environment, min(start, end), max(start, end), page, page),
    len(abs(end - start) - 1),
    prog(progress),
    prim(0),
    rend(true),
    init(false),
    border(false) {
  if (abs(end - start) < 5) while (1);
}

void ProgressBar::style(bool s) {
  if (s != border) {
    border = s;
    init = false;
    prim = 0;
  }
}

void ProgressBar::set(uint8_t progress) {
  if (prog != progress) {
    prog = min(progress, 100);
    rend = (prog != prim);
  }
}

uint8_t ProgressBar::get() {
  return prog;
}


void ProgressBar::render() {
  if (rend || !init) {
    env -> set_pos(x1, y1);
    int8_t end = (len * prog) / 100, start = (len * prim) / 100;
    uint8_t bord = (!border) * 0b11111111;
    if (!init) env -> send_data(0b00111100 | bord);
    if (end > start) {
      env -> set_pos(x1 + start + 1, y1);
      if (start == 0) env -> send_data(0b01111110 | bord);
      env -> send_pack(0b11111111, end - start - (start == 0) - (end == len));
      if (end == len) {
        if (border)
          env -> send_data(0b01111110);
        else
          env -> send_data(0b11111111);
      }
    } else if (end < start) {
      env -> set_pos(x1 + end + 1, y1);
      if (end == 0) {
        if (border)
          env -> send_data(0b01000010);
        else
          env -> send_data(0b10000001);
      }
      env -> send_pack(0b10000001, start - end - (end == 0) - (start == len));
      if (start == len) {
        if (border)
          env -> send_data(0b01000010);
        else
          env -> send_data(0b10000001);
      }
    }
    if (!init) {
      if (border) env -> send_data(0b01000010);
      env -> send_pack(0b10000001, len - end - 1 - border);
      if (border) env -> send_data(0b01000010);
      else env -> send_data(0b10000001);
      env -> send_data(0b00111100 | bord);
    }
    rend = false;
    init = true;
    prim = prog;
  }
}

//=======================================
void Toggle::buf(uint8_t len, uint8_t inn) {
  if (border) {
    env->send_data(0b10011001 | inn);
    env->send_pack(0b10111101 | inn, len / 2 - 3);
    env->send_data(0b10011001 | inn);
  } else {
    env->send_pack(0b10111101 | inn, len / 2 - 1);
  }
}

Toggle::Toggle(Environment& environment, uint8_t page, uint8_t start, uint8_t end, bool state)
  : Widget(environment, min(start, end), max(start, end), page, page),
    len(abs(end - start) - 1),
    st(state),
    rend(true),
    init(false),
    border(false),
    inner(false) {
  if (abs(end - start) < 10) while (1);
}

void Toggle::set(bool state) {
  if (st != state) {
    rend = !rend;
    st = state;
  }
}

uint8_t Toggle::get() {
  return st;
}

void Toggle::style(bool s, bool s2) {
  if (s != border) {
    border = s;
    rend = true;
  }
  if (s2 != inner) {
    inner = s2;
    rend = true;
  }
}

void Toggle::render() {
  if (rend || !init) {
    env -> set_pos(x1, y1);
    uint8_t bord = (!border) * 0b11111111, inn = (!inner) * 0b11111111;
    env -> send_data(0b00111100 | bord);
    if (inner || st)
      if (border)
        env -> send_data(0b01000010);
      else
        env -> send_data(0b10000001);
    else
      env -> send_data(0b01111110 | bord);
    if (!st)
      buf(len, inn);
    else
      env -> send_pack(0b10000001, len / 2 - 1 + len % 2);
    if (st)
      buf(len, inn);
    else
      env -> send_pack(0b10000001, len / 2 - 1 + len % 2);
    if (inner || !st)
      if (border)
        env -> send_data(0b01000010);
      else
        env -> send_data(0b10000001);
    else
      env -> send_data(0b01111110 | bord);
    env -> send_data(0b00111100 | bord);
    rend = false;
    init = true;
  }
}

//=======================================
Text::Text(Environment& environment, uint8_t page_start, uint8_t page_end, uint8_t start, uint8_t end, String text)
  : Widget(environment, min(start, end), max(page_start, page_end), min(page_start, page_end), max(page_start, page_end)),
    len(abs(end - start) + 1), h(abs(page_end - page_start) + 1), txt(text) {
  txt.toUpperCase();
}

void Text::set(String text) {
  if (txt != text) {
    txt = text;
    txt.toUpperCase();
  }
}

bool Text::draw_char(uint8_t& pos, uint8_t& page, char c) {
  if (page > y2) return true;
  uint8_t index = 0;
  if (c >= '!' && c <= '?') {
    if (len - pos < 4) {
      page++;
      pos = x1;
      return false;
    }
    env->set_pos(pos, page);
    pos += 4;
    index = c - '!';
    uint8_t data[3] = {pgm_read_byte(&chars[index][0]), pgm_read_byte(&chars[index][1]), pgm_read_byte(&chars[index][2])};
    env->send_mult(data, 3);
  } else if (c >= 'A' && c <= 'Z') {
    if (len - pos < 6) {
      page++;
      pos = x1;
      return false;
    }
    env->set_pos(pos, page);
    pos += 6;
    index = c - 'A';
    uint8_t data[5] = {pgm_read_byte(&chars_l[index][0]), pgm_read_byte(&chars_l[index][1]),
                       pgm_read_byte(&chars_l[index][2]), pgm_read_byte(&chars_l[index][3]),
                       pgm_read_byte(&chars_l[index][4])};
    env->send_mult(data, 5);
  } else if (c == ' ') {
    if (len - pos < 2) {
      page++;
      pos = x1;
      return false;
    }
    env->set_pos(pos, page);
    pos += 2;
    env->send_data(0);
  } else if ((int)(unsigned char)c == (int)(unsigned char)'Ё') {
    if (len - pos < 6) {
      page++;
      pos = x1;
      return false;
    }
    env->set_pos(pos, page);
    pos += 6;
    uint8_t data[5] = {0b01111100, 0b01010110, 0b01010100, 0b01010110, 0b01010100};
    env->send_mult(data, 5);
  } else if (((int)(unsigned char)c >= (int)(unsigned char)'А' &&
             (int)(unsigned char)c <= (int)(unsigned char)'Я') || ((int)(unsigned char)c >= (int)(unsigned char)'а' &&
             (int)(unsigned char)c <= (int)(unsigned char)'я')) {
    if (len - pos < 6) {
      page++;
      pos = x1;
      return false;
    }
    env->set_pos(pos, page);
    pos += 6;
    if ((int)(unsigned char)c >= (int)(unsigned char)'А' && (int)(unsigned char)c <= (int)(unsigned char)'Я') 
      index = c - (int)(unsigned char)'А'; 
    else
      index = c - (int)(unsigned char)'а';
    uint8_t data[5] = {pgm_read_byte(&chars_r[index][0]), pgm_read_byte(&chars_r[index][1]),
                       pgm_read_byte(&chars_r[index][2]), pgm_read_byte(&chars_r[index][3]),
                       pgm_read_byte(&chars_r[index][4])};
    env->send_mult(data, 5);
  }
  return true;
}

void Text::render() {
  uint8_t pos = x1, page = y1;
  for (uint8_t i = 0; i <= txt.length(); i += draw_char(pos, page, txt[i])) {}
}
