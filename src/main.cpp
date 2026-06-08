#include <display.h>
#include <draw.h>
#include <framebuffer.h>
#include <model.h>

#include <chrono>
#include <iostream>

constexpr int WINDOW_WIDTH = 960;
constexpr int WINDOW_HEIGHT = 960;

constexpr Color black = {0, 0, 0, 255};
constexpr Color white = {255, 255, 255, 255};
constexpr Color green = {0, 255, 0, 255};

int main(int argc, char** argv) {
  init_display(WINDOW_WIDTH, WINDOW_HEIGHT);
  Framebuffer image(WINDOW_WIDTH, WINDOW_HEIGHT, black);
  Viewport vp{100, 100, WINDOW_WIDTH - 100, WINDOW_HEIGHT - 100};

  std::string model_filepath = "obj/boggie/body.obj";
  Model model(model_filepath);
  std::cerr << "Loaded " << model.nverts() << " verts, " << model.nfaces()
            << " faces" << std::endl;

  using clock = std::chrono::high_resolution_clock;
  auto last_frame_start = clock::now();
  while (poll_events()) {
    auto now = clock::now();
    float dt = std::chrono::duration_cast<std::chrono::duration<float>>(
                   now - last_frame_start)
                   .count();
    last_frame_start = now;
    std::cout << "\rfps: " << dt << "\t" << std::flush;

    for (const std::vector<int>& face : model.faces) {
      for (size_t i = 0; i < face.size(); i++) {
        vml::Vec3f v0 = model.verts[face[i]];
        vml::Vec3f v1 = model.verts[face[(i + 1) % face.size()]];
        line(apply_viewport(vp, v0), apply_viewport(vp, v1), image, green);
      }
    }

    update_display(image);
  }

  destroy_display();
}
