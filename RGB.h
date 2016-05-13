#ifndef _RGB_H
#define _RGB_H

#include <Arduino.h>

#define COMPONENT_MAX 255

#define RGB_RED RGB(255, 0, 0)
#define RGB_YELLOW RGB(255, 255, 0)
#define RGB_GREEN RGB(0, 255, 0)
#define RGB_CYAN RGB(0, 255, 255)
#define RGB_BLUE RGB(0, 0, 255)
#define RGB_MAGENTA RGB(255, 0, 255)

struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  RGB(unsigned long value) : r(value >> 16), g(value >> 8), b(value) {}
  RGB(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}
  RGB() : r(0), g(0), b(0) {}
  RGB(float hue, float brightness) {
    hue = fmod(hue, 1.0) * 6.0;
    float hueFraction = fmod(hue, 1.0);
    RGB color;
    if (hue >= 5) color = RGB_MAGENTA.interpolate(RGB_RED, hueFraction);
    else if (hue >= 4) color = RGB_BLUE.interpolate(RGB_MAGENTA, hueFraction);
    else if (hue >= 3) color = RGB_CYAN.interpolate(RGB_BLUE, hueFraction);
    else if (hue >= 2) color = RGB_GREEN.interpolate(RGB_CYAN, hueFraction);
    else if (hue >= 1) color = RGB_YELLOW.interpolate(RGB_GREEN, hueFraction);
    else color = RGB_RED.interpolate(RGB_YELLOW, hueFraction);
    r = color.r * brightness;
    g = color.g * brightness;
    b = color.b * brightness;
  }

  inline RGB operator*(float multiplier) {
    return RGB(r * multiplier, g * multiplier, b * multiplier);
  }

  inline RGB operator<<(RGB other) {
    return RGB(max(r, other.r), max(g, other.g), max(b, other.b));
  }

  inline RGB operator+(RGB other) {
    return RGB(min(r + other.r, COMPONENT_MAX), min(g + other.g, COMPONENT_MAX), min(b + other.b, COMPONENT_MAX));
  }

  RGB logged() {
    return RGB(r * r / 256, g * g / 256, b * b / 256);
  }

  RGB adjusted() {
    return RGB(r * r / 255, g * g / 255, b * b / 255);
  }

  // interpolate(RGB other, float m)
  //   m: fraction of the way from self to other
  RGB interpolate(RGB other, float m) {
    float m_ = 1.0 - m;
    return RGB(round(r * m_ + other.r * m), round(g * m_ + other.g * m), round(b * m_ + other.b * m));
  }

  void print() {
    Serial.print(r);
    Serial.print(", ");
    Serial.print(g);
    Serial.print(", ");
    Serial.println(b);
  }
};

#endif