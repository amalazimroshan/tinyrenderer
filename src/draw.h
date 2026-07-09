#pragma once
#include <cmath>
#include <vector>
#include <vml/matrix.hpp>
#include <vml/vector.hpp>

#include "framebuffer.h"
#include "model.h"

enum class rendering_style {
  wireframe,
  facet_shading,
};

struct draw_command {
  enum rendering_style rendering_style;
  Color wireframe_color;
  Color faceset_shading_color;
  vml::Mat4f transform;
};
struct Viewport {
  int xmin, ymin, xmax, ymax;
};

inline float det2D(vml::Vec2f a, vml::Vec2f b) { return a.x * b.y - a.y * b.x; }

vml::Vec2i apply_viewport(const Viewport& vp, vml::Vec4f v);
void line(vml::Vec2i p1, vml::Vec2i p2, Framebuffer& fb, Color color);
vml::Vec2i world2screen(vml::Vec3f v, int width, int height);
void draw(const Model& model, Framebuffer& fb, const Viewport& vp,
          const draw_command& dc);
