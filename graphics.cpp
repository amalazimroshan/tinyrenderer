#include "graphics.h"

#include "geometry.h"
void line(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor color) {
  bool steep = false;
  if (std::abs(t0.x - t1.x) < std::abs(t0.y - t1.y)) {
    std::swap(t0.x, t0.y);
    std::swap(t1.x, t1.y);
    steep = true;
  }
  if (t0.x > t1.x) {
    std::swap(t0.x, t1.x);
    std::swap(t0.y, t1.y);
  }
  int dx = t1.x - t0.x;
  int dy = t1.y - t0.y;
  int derror = std::abs(dy) * 2;
  float error = 0;
  int y = t0.y;
  for (int x = t0.x; x <= t1.x; x++) {
    if (steep) {
      image.set(y, x, color);
    } else {
      image.set(x, y, color);
    }
    error += derror;
    if (error > dx) {
      y += (t1.y > t0.y) ? 1 : -1;
      error -= dx * 2;
    }
  }
}

// triangle no fill///
void triangle_nofill(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image,
                     TGAColor color) {
  line(t0, t1, image, color);
  line(t1, t2, image, color);
  line(t2, t0, image, color);
}
////////// standard traingle fill ///////////////////
void fillBottomTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image,
                        TGAColor color) {
  float invslope1 = float(t1.x - t0.x) / float(t1.y - t0.y);
  float invslope2 = float(t2.x - t0.x) / float(t2.y - t0.y);
  float cursor1 = t0.x;
  float cursor2 = t0.x;
  for (int scanlineY = t0.y; scanlineY <= t1.y; scanlineY++) {
    line(Vec2i((int)cursor1, scanlineY), Vec2i((int)cursor2, scanlineY), image,
         color);
    cursor1 += invslope1;
    cursor2 += invslope2;
  }
}
void fillTopTriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image,
                     TGAColor color) {
  float invslope1 = float(t1.x - t0.x) / float(t1.y - t0.y);
  float invslope2 = float(t2.x - t0.x) / float(t2.y - t0.y);
  float cursor1 = t0.x;
  float cursor2 = t0.x;
  for (int scanlineY = t0.y; scanlineY > t1.y; scanlineY--) {
    line(Vec2i((int)cursor1, scanlineY), Vec2i((int)cursor2, scanlineY), image,
         color);
    cursor1 -= invslope1;
    cursor2 -= invslope2;
  }
}
void standard_triangle_fill(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image,
                            TGAColor color) {
  if (t0.y > t1.y) std::swap(t0, t1);
  if (t0.y > t2.y) std::swap(t0, t2);
  if (t1.y > t2.y) std::swap(t1, t2);
  line(t0, t1, image, color);
  line(t1, t2, image, color);
  line(t2, t0, image, color);

  float t = float(t1.y - t0.y) / (t2.y - t0.y);
  int x3 = t0.x + t + t * (t2.x - t0.x);
  Vec2i t3 = Vec2i(x3, t1.y);
  // std::cout << "(" << x3 << ", " << t1.y <<")"<< std::endl;
  fillBottomTriangle(t0, t1, t3, image, color);
  fillTopTriangle(t2, t1, t3, image, color);
}
///////////////////////////////////////////////

void scanline_triangle_fill(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image,
                            TGAColor color) {
  line(t0, t1, image, color);
  line(t1, t2, image, color);
  line(t2, t0, image, color);
  Vec2i bottomLeft(std::min(std::min(t0.x, t1.x), t2.x),
                   std::min(std::min(t0.y, t1.y), t2.y));
  Vec2i topRight(std::max(std::max(t0.x, t1.x), t2.x),
                 std::max(std::max(t0.y, t1.y), t2.y));

  for (int i = bottomLeft.y; i <= topRight.y; i++) {
    int minX = topRight.x + 1;
    int maxX = bottomLeft.x - 1;
    for (int j = bottomLeft.x; j <= topRight.x; j++) {
      TGAColor currentColor = image.get(j, i);
      if (currentColor == color) {
        minX = std::min(minX, j);
        maxX = std::max(maxX, j);
      }
    }
    line(Vec2i(minX, i), Vec2i(maxX, i), image, color);
  }
}
Vec3f barycentric(const Vec3f* A, const Vec3f* B, const Vec3f* C,
                  const Vec3f* P) {
  Vec3f AB = *B - *A;
  Vec3f AC = *C - *A;
  Vec3f AP = *P - *A;
  Vec3f crossAC_AB = cross(AC, AB);
  Vec3f crossAP_AB = cross(AP, AB);
  Vec3f crossAC_AP = cross(AC, AP);
  float u = crossAP_AB.norm() / crossAC_AB.norm();
  float v = crossAC_AP.norm() / crossAC_AB.norm();
  float w = 1.0f - u - v;
  return Vec3f(u, v, w);
}
void barycentricScanline_triangle_fill(Vec3f* pts, float* zbuffer,
                                       TGAImage& image, TGAColor color) {
  Vec2f bboxmin(std::numeric_limits<float>::max(),
                std::numeric_limits<float>::max());
  Vec2f bboxmax(-std::numeric_limits<float>::max(),
                -std::numeric_limits<float>::max());
  Vec2f clamp(image.get_width() - 1, image.get_height() - 1);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      bboxmin[j] = std::max(0.0f, std::min(bboxmin[j], pts[i][j]));
      bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
    }
  }
  Vec3f P;
  for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
    for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
      Vec3f bc_screen = barycentric(&pts[0], &pts[1], &pts[2], &P);
      if (bc_screen[0] < 0 || bc_screen[1] < 0 || bc_screen[2] < 0) continue;
      P.z = 0;
      for (int i = 0; i < 3; i++) P.z += pts[i][2] * bc_screen[i];
      if (zbuffer[int(P.x + P.y * 800)] < P.z) {
        zbuffer[int(P.x + P.y * 800)] = P.z;
        image.set(P.x, P.y, color);
      }
    }
  }
  // Vec2i vs1(t1 - t0);
  // Vec2i vs2(t2 - t0);
  // for (int x = minX; x <= maxX; x++) {
  //   for (int y = minY; y <= maxY; y++) {
  //     Vec2i q = Vec2i(Vec2i(x, y) - t0);
  //     float s = (q ^ vs2) / (vs1 ^ vs2);
  //     float t = (vs1 ^ q) / (vs1 ^ vs2);
  //     if ((s >= 0) && (t >= 0) && (s + t <= 1)) {
  //       image.set(x, y, color);
  //     }
  //   }
  // }
}