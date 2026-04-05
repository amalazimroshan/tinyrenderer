#include <SDL2/SDL.h>
#include <display.h>
#include <framebuffer.h>
#include <tgaimage.h>

#include <cmath>

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;

const TGAColor black = {0, 0, 0, 0};
const TGAColor white = {255, 255, 255, 255};

int main(int argc, char** argv) {
  init_display(WINDOW_WIDTH, WINDOW_HEIGHT);

  Framebuffer image = Framebuffer(WINDOW_WIDTH, WINDOW_HEIGHT, black);
  image.set(5, 5, white);

  while (poll_events()) {
    update_display(image);
  }

  destroy_display();
}