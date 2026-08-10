#include "framebuffer.h"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <limits>

Framebuffer::Framebuffer(const int w, const int h, const Color& fill)
    : w(w), h(h), data(w * h * 4), depth(w * h) {
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
  std::fill(std::begin(depth), std::end(depth),
            std::numeric_limits<float>::infinity());
}

const std::uint8_t* Framebuffer::buffer() const { return data.data(); }

float& Framebuffer::depth_at(int x, int y) {
  if (x < 0 || y < 0 || x >= w || y >= h) {
    throw std::out_of_range("Framebuffer::depth_at: index out of bounds");
  }
  return depth[x + y * w];
}
