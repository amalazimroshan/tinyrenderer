#include <iostream>

#include "display.h"
#include "geometry.h"
#include "graphics.h"
#include "model.h"
#include "tgaimage.h"

Model* model = NULL;
const int width = 800;
const int height = 800;

float crossProduct(const Vec2i& v1, const Vec2i& v2) {
  return static_cast<float>(v1.x * v2.y - v1.y * v2.x);
}

float calculateArea(const Vec2i& v1, const Vec2i& v2, const Vec2i& v3) {
  return std::abs(crossProduct(v2 - v1, v3 - v1)) / 2.0f;
}
int main(int argc, char* args[]) {
  TGAImage image(width, height, TGAImage::RGB);
  Vec2i t0[3] = {Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80)};
  Vec2i t1[3] = {Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180)};
  Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
  triangle_nofill(t0[0], t0[1], t0[2], image, blue);

  Vec2i inside(55, 115);
  Vec2i outside(60, 140);

  float totalArea = calculateArea(t0[0], t0[1], t0[2]);
  float aArea = calculateArea(t0[0], t0[1], outside);
  float bArea = calculateArea(t0[1], t0[2], outside);
  float cArea = calculateArea(t0[2], t0[0], outside);

  float alpha = aArea / totalArea;
  float beta = bArea / totalArea;
  float gamma = cArea / totalArea;

  std::cout << totalArea << '\n';
  std::cout << aArea << '\n';
  std::cout << bArea << '\n';
  std::cout << cArea << '\n';
  std::cout << alpha << " " << beta << " " << gamma << '\n';

  image.flip_vertically();
  image.write_tga_file("output.tga");
  if (!image.read_tga_file("output.tga")) {
    std::cerr << "Failed to read TGA file." << std::endl;
    return -1;
  }
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  if (!create_window(image.get_width(), image.get_height(), window, renderer)) {
    return -1;
  }
  display_tga_image(renderer, image);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}