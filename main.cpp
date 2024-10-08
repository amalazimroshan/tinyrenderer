#include <SDL2/SDL.h>
#include <math.h>

#include <iostream>

#include "Renderer.h"

typedef mat<3, 3, float> Mat3f;
bool edgeFunction(Vec2f a, Vec2f b, Vec2f p) {
  Vec2f ab = b - a;
  Vec2f ap = a - p;
  Vec3f cp = cross(Vec3f(ab.x, ab.y, 0), Vec3f(ap.x, ap.y, 0));
  return cp.z > 0;
}
void renderTriangle(Renderer renderer, Vec2f A, Vec2f B, Vec2f C) {
  Vec2f minbbox((std::min(std::min(A.x, B.x), C.x)),
                (std::min(std::min(A.y, B.y), C.y)));
  Vec2f maxbbox((std::max(std::max(A.x, B.x), C.x)),
                (std::max(std::max(A.y, B.y), C.y)));
  Vec2f P;
  for (P.y = minbbox.y; P.y <= maxbbox.y; P.y++) {
    for (P.x = minbbox.x; P.x <= maxbbox.x; P.x++) {
      if (edgeFunction(A, B, P) && edgeFunction(B, C, P) &&
          edgeFunction(C, A, P)) {
        renderer.DrawPoint(P.x, P.y, 0x00FFFF);
      }
    }
  }
}
Mat3f translateMatrix(float tx, float ty) {
  Mat3f m;
  m[0] = Vec3f(1, 0, tx);
  m[1] = Vec3f(0, 1, ty);
  m[2] = Vec3f(0, 0, 1);
  return m;
}
Mat3f scaleMatrix(float sx, float sy) {
  Mat3f m;
  m[0] = Vec3f(sx, 0, 0);
  m[1] = Vec3f(0, sy, 0);
  m[2] = Vec3f(0, 0, 1);
  return m;
}
Mat3f rotateMatrix(float angle) {
  float c = std::cos(angle);
  float s = std::cos(angle);
  Mat3f m;
  m[0] = Vec3f(c, -s, 0);
  m[1] = Vec3f(s, c, 0);
  m[2] = Vec3f(0, 0, 1);
  return m;
}
Mat3f perspectiveMatrix() {
  Mat3f m;
  m[0] = Vec3f(1, 0, 0);
  m[1] = Vec3f(0, 1, 0);
  m[2] = Vec3f(1 / 2, 0, 1);
  return m;
}
Vec3f toHomogenous(const Vec2f& v) { return Vec3f(v.x, v.y, 1.0f); };
Vec2f fromHomogenous(const Vec3f& v) { return Vec2f(v.x / v.z, v.y / v.z); }
void transformTriangle(Vec2f (&triangle)[3], const Mat3f& transform) {
  for (int i = 0; i < 3; i++) {
    Vec3f homogenous = toHomogenous(triangle[i]);
    Vec3f transformed = transform * homogenous;
    triangle[i] = fromHomogenous(transformed);
  }
}

int main() {
  Renderer renderer;
  renderer.Initialize();

  bool isRunning = true;
  SDL_Event event;
  Vec2f triangleA[3] = {
      Vec2f(100, 100),
      Vec2f(50, 400),
      Vec2f(200, 200),
  };

  std::cout << "-------------------" << std::endl
            << *triangleA << triangleA[1] << triangleA[2] << std::endl;

  Mat3f translation = translateMatrix(300, 4);
  Mat3f rotate = rotateMatrix(45);
  Mat3f perspective = perspectiveMatrix();
  Mat3f scale = scaleMatrix(2, 2);
  Mat3f transformMatrix =  //
      translation          //
      * perspective
      // * rotate             //
      // * scale              //
      ;
  transformTriangle(triangleA, transformMatrix);

  std::cout << "-------------------" << std::endl
            << *triangleA << triangleA[1] << triangleA[2] << std::endl;
  while (isRunning) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) isRunning = false;
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
      }
      renderer.Clear();
      renderTriangle(renderer, triangleA[0], triangleA[1], triangleA[2]);
      renderer.Present();
    }
  }
  renderer.Shutdown();
  return 0;
}