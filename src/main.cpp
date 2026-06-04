#include <display.h>
#include <draw.h>
#include <framebuffer.h>
#include <model.h>

#include <iostream>

constexpr int WINDOW_WIDTH = 960;
constexpr int WINDOW_HEIGHT = 960;

constexpr Color black = {0, 0, 0, 255};
constexpr Color white = {255, 255, 255, 255};
constexpr Color green = {0, 255, 0, 255};

int main(int argc, char** argv) {
  init_display(WINDOW_WIDTH, WINDOW_HEIGHT);
  Framebuffer image(WINDOW_WIDTH, WINDOW_HEIGHT, black);

  std::string model_filepath = "obj/boggie/body.obj";
  Model model(model_filepath);
  std::cerr << "Loaded " << model.nverts() << " verts, " << model.nfaces()
            << " faces" << std::endl;

  for (const std::vector<int>& face : model.faces) {
    for (size_t i = 0; i < face.size(); i++) {
      vml::Vec3f v0 = model.verts[face[i]];
      vml::Vec3f v1 = model.verts[face[(i + 1) % face.size()]];
      line(world2screen(v0, WINDOW_WIDTH, WINDOW_HEIGHT),
           world2screen(v1, WINDOW_WIDTH, WINDOW_HEIGHT), image, green);
    }
  }

  while (poll_events()) {
    update_display(image);
  }

  destroy_display();
}
