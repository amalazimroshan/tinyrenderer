#include "draw.h"

#include <cmath>

void line(vml::Vec2i p1, vml::Vec2i p2, Framebuffer& fb, Color color) {
  bool steep = std::abs(p1.x - p2.x) < std::abs(p1.y - p2.y);
  if (steep) {
    std::swap(p1.x, p1.y);
    std::swap(p2.x, p2.y);
  }
  if (p1.x > p2.x) {
    std::swap(p1.x, p2.x);
    std::swap(p1.y, p2.y);
  }
  int dx = p2.x - p1.x;
  int dy = std::abs(p2.y - p1.y);
  int y = p1.y;
  int ierror = 0;
  for (int x = p1.x; x <= p2.x; x++) {
    if (steep)
      fb.set(y, x, color);
    else
      fb.set(x, y, color);
    ierror += 2 * dy;
    if (ierror > dx) {
      y += p2.y > p1.y ? 1 : -1;
      ierror -= 2 * dx;
    }
  }
}

vml::Vec2i world2screen(vml::Vec3f v, int width, int height) {
  int sx = (v.x + 1.f) / 2.f * width;
  int sy = ((-v.y) + 1.f) / 2.f * height;
  return vml::Vec2i(sx, sy);
}

vml::Vec2i apply_viewport(const Viewport& vp, vml::Vec3f v) {
  int x = vp.xmin + (vp.xmax - vp.xmin) * (0.5f + 0.5f * v.x);
  int y = vp.ymin + (vp.ymax - vp.ymin) * (0.5f - 0.5f * v.y);
  return vml::Vec2i(x, y);
}