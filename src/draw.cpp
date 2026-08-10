#include "draw.h"

#include <algorithm>
#include <cstdlib>
#include <utility>

#include "vml/vector.hpp"

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

vml::Vec3f apply_viewport(const Viewport& vp, vml::Vec3f v) {
  float x = vp.xmin + (vp.xmax - vp.xmin) * (0.5f + 0.5f * v.x);
  float y = vp.ymin + (vp.ymax - vp.ymin) * (0.5f - 0.5f * v.y);
  float z = v.z;
  return vml::Vec3f(x, y, z);
}

vml::Vec3f persp(vml::Vec4f v) {
  constexpr float c = 3;
  const float deno = (1 - v.z / c);
  // const float deno = 1;
  return vml::Vec3f{v.x / deno, v.y / deno, v.z / deno};
}

void draw(const Model& model, Framebuffer& fb, const Viewport& vp,
          const draw_command& dc) {
  for (int i = 0; i < (int)model.faces.size(); ++i) {
    const auto& face = model.faces[i];
    vml::Vec4f vw0 = dc.transform * vml::Vec4f(model.verts[face[0]], 1.f);
    vml::Vec4f vw1 = dc.transform * vml::Vec4f(model.verts[face[1]], 1.f);
    vml::Vec4f vw2 = dc.transform * vml::Vec4f(model.verts[face[2]], 1.f);

    vml::Vec3f s0 = apply_viewport(vp, persp(vw0));
    vml::Vec3f s1 = apply_viewport(vp, persp(vw1));
    vml::Vec3f s2 = apply_viewport(vp, persp(vw2));
    // Color fill = colorFromIndex(i);
    Color fill = dc.faceset_shading_color;
    switch (dc.rendering_style) {
      // case rendering_style::wireframe:
      //   line(s0, s1, fb, dc.wireframe_color);
      //   line(s1, s2, fb, dc.wireframe_color);
      //   line(s2, s0, fb, dc.wireframe_color);
      //   break;
      case rendering_style::facet_shading: {
        int x_min = std::max(0.f, std::min({s0.x, s1.x, s2.x}));
        int y_min = std::max(0.f, std::min({s0.y, s1.y, s2.y}));
        int x_max = std::min(fb.width() - 1.f, std::max({s0.x, s1.x, s2.x}));
        int y_max = std::min(fb.height() - 1.f, std::max({s0.y, s1.y, s2.y}));

        vml::Vec2f f0(s0.x, s0.y), f1(s1.x, s1.y), f2(s2.x, s2.y);

        bool ccw = det2D(f1 - f0, f2 - f0) < 0.f;
        if (ccw) continue;

        float d012 = det2D(f1 - f0, f2 - f0);
        for (int y = y_min; y <= y_max; ++y) {
          for (int x = x_min; x <= x_max; ++x) {
            vml::Vec2f p(x + 0.5f, y + 0.5f);

            float d01p = det2D(f1 - f0, p - f0);
            float d12p = det2D(f2 - f1, p - f1);
            float d20p = det2D(f0 - f2, p - f2);

            if ((d01p >= 0 && d12p >= 0 && d20p >= 0) ||
                (d01p <= 0 && d12p <= 0 && d20p <= 0)) {
              float l0 = d12p / d012;
              float l1 = d20p / d012;
              float l2 = d01p / d012;
              float z = l0 * s0.z + l1 * s1.z + l2 * s2.z;
              float& zref = fb.depth_at(x, y);
              if (zref > z) {
                fill = Color(l0 * 255, l1 * 255, l2 * 255, 255);
                zref = z;
                fb.set(x, y, fill);
              }
            }
          }
        }
        break;
      }
    }
  }
}
