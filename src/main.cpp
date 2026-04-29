#include <SDL2/SDL.h>
#include <display.h>
#include <framebuffer.h>
#include <tgaimage.h>

#include <cmath>
#include <ctime>
#include <vml/vector.hpp>

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;

constexpr TGAColor black = {0, 0, 0, 255};
constexpr TGAColor white = {255, 255, 255, 255};
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

void line(vml::Vec2i p1, vml::Vec2i p2, Framebuffer& framebuffer,
          TGAColor color) {
  bool steep = std::abs(p1.x - p2.x) < std::abs(p1.y - p2.y);
  if (steep) {
    std::swap(p1.x, p1.y);
    std::swap(p2.x, p2.y);
  }
  if (p1.x > p2.x) {
    std::swap(p1.x, p2.x);
    std::swap(p1.y, p2.y);
  }
  int y = p1.y;
  float error = 0;
  for (int x = p1.x; x <= p2.x; x++) {
    if (steep)
      framebuffer.set(y, x, color);
    else
      framebuffer.set(x, y, color);
    error += (p2.y - p1.y) / static_cast<float>(p2.x - p1.x);
    if (error > 0.5) {
      y += p2.y > p1.y ? 1 : -1;
      error -= 1.0;
    }
  }
}

int main(int argc, char** argv) {
  init_display(WINDOW_WIDTH, WINDOW_HEIGHT);

  Framebuffer image = Framebuffer(WINDOW_WIDTH, WINDOW_HEIGHT, black);

  std::srand(std::time({}));
  for (int i = 0; i < (1 << 24); i++) {
    vml::Vec2i p1(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
    vml::Vec2i p2(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
    line(
        p1, p2, image,
        {static_cast<uint8_t>(rand() % 255), static_cast<uint8_t>(rand() % 255),
         static_cast<uint8_t>(rand() % 255),
         static_cast<uint8_t>(rand() % 255)});
  }

  line(vml::Vec2i(20, 30), vml::Vec2i(540, 400), image, blue);
  line(vml::Vec2i(540, 400), vml::Vec2i(620, 70), image, blue);
  line(vml::Vec2i(620, 70), vml::Vec2i(20, 30), image, blue);

  while (poll_events()) {
    update_display(image);
  }

  destroy_display();
}