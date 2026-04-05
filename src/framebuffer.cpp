#include "framebuffer.h"

Framebuffer::Framebuffer(const int w, const int h, const TGAColor& fill)
    : w(w), h(h), data(w * h * 4) {
  clear(fill);
}

void Framebuffer::set(const int x, const int y, const TGAColor& c) {
  if (x < 0 || y < 0 || x >= w || y >= h) return;
  std::memcpy(data.data() + (x + y * w) * 4, c.bgra, 4);
}

TGAColor Framebuffer::get(const int x, const int y) const {
  if (x < 0 || y < 0 || x >= w || y >= h) return {};
  TGAColor ret = {0, 0, 0, 0, 4};
  const std::uint8_t* p = data.data() + (x + y * w) * 4;
  for (int i = 0; i < 3; i++) ret.bgra[i] = p[i];
  return ret;
}

int Framebuffer::width() const { return w; }

int Framebuffer::height() const { return h; }

void Framebuffer::clear(const TGAColor& c) {
  for (int i = 0; i < w * h; i++) std::memcpy(data.data() + i * 4, c.bgra, 4);
}

const std::uint8_t* Framebuffer::buffer() const { return data.data(); }
