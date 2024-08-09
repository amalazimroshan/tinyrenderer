#include "graphics.h"

#include "geometry.h"

float calculateArea(const Vec2i& v1, const Vec2i& v2, const Vec2i& v3) {
  return std::abs((v2 - v1) ^ (v3 - v1)) / 2.0f;
}
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
    bool foundMatch = false;

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

void BarycentricScanline_triangle_fill(Vec2i t0, Vec2i t1, Vec2i t2,
                                       TGAImage& image, TGAColor color) {
  int minX = std::min(std::min(t0.x, t1.x), t2.x);
  int minY = std::min(std::min(t0.y, t1.y), t2.y);
  int maxX = std::max(std::max(t0.x, t1.x), t2.x);
  int maxY = std::max(std::max(t0.y, t1.y), t2.y);
  Vec2i vs1(t1 - t0);
  Vec2i vs2(t2 - t0);
  for (int x = minX; x <= maxX; x++) {
    for (int y = minY; y <= maxY; y++) {
      Vec2i q = Vec2i(Vec2i(x, y) - t0);
      float s = (q ^ vs2) / (vs1 ^ vs2);
      float t = (vs1 ^ q) / (vs1 ^ vs2);
      if ((s >= 0) && (t >= 0) && (s + t <= 1)) {
        image.set(x, y, color);
      }
    }
  }
}