#pragma once
#include <vml/vector.hpp>

#include "framebuffer.h"

struct Viewport {
  int xmin, ymin, xmax, ymax;
};

vml::Vec2i apply_viewport(const Viewport& vp, vml::Vec3f v);
void line(vml::Vec2i p1, vml::Vec2i p2, Framebuffer& fb, Color color);
vml::Vec2i world2screen(vml::Vec3f v, int width, int height);
