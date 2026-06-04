#include "framebuffer.h"

Framebuffer::Framebuffer(const int w, const int h, const Color& fill)
    : w(w), h(h), data(w * h * 4) {
  clear(fill);
}

Color Framebuffer::get(const int x, const int y) const {
  if (x < 0 || y < 0 || x >= w || y >= h) return {};
  Color ret;
  std::memcpy(&ret, (data.data() + (x + y * w) * 4), sizeof(Color));
  return ret;
}

int Framebuffer::width() const { return w; }
int Framebuffer::height() const { return h; }

void Framebuffer::clear(const Color& c) {
  for (int i = 0; i < w * h; i++) std::memcpy(data.data() + i * 4, &c.r, 4);
}

const std::uint8_t* Framebuffer::buffer() const { return data.data(); }
