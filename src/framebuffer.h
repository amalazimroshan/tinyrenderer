#pragma once
#include <cstdint>
#include <cstring>
#include <vector>

#include "tgaimage.h"

struct Framebuffer {
  int w, h;
  std::vector<std::uint8_t> data;

  Framebuffer(const int w, const int h, const TGAColor& fill);

  void set(const int x, const int y, const TGAColor& c) {
    if (x < 0 || y < 0 || x >= w || y >= h) return;
    std::memcpy(data.data() + (x + y * w) * 4, c.bgra, 4);
  }

  TGAColor get(const int x, const int y) const;
  int width() const;
  int height() const;
  void clear(const TGAColor& c);
  const std::uint8_t* buffer() const;
};
