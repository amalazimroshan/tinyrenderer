#pragma once
#include <cstdint>
#include <cstring>
#include <vector>

#include "color.h"

struct Framebuffer {
  int w, h;
  std::vector<std::uint8_t> data;

  Framebuffer(const int w, const int h, const Color& fill);

  void set(const int x, const int y, const Color& c) {
    if (x < 0 || y < 0 || x >= w || y >= h) return;
    std::memcpy(data.data() + (x + y * w) * 4, &c.r, 4);
  }

  Color get(const int x, const int y) const;
  int width() const;
  int height() const;
  void clear(const Color& c);
  const std::uint8_t* buffer() const;
};
