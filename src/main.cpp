#include <algorithm>
#include <chrono>
#include <iostream>

#include "color.h"
#include "display.h"
#include "draw.h"
#include "framebuffer.h"
#include "model.h"
#include "transform.h"
#include "vml/matrix.hpp"
#include "vml/vector.hpp"

constexpr int WINDOW_WIDTH = 960;
constexpr int WINDOW_HEIGHT = 960;

int main(int argc, char** argv) {
  init_display(WINDOW_WIDTH, WINDOW_HEIGHT);
  Framebuffer image(WINDOW_WIDTH, WINDOW_HEIGHT, BLACK);
  Viewport vp{100, 100, WINDOW_WIDTH - 100, WINDOW_HEIGHT - 100};

  std::string model_filepath = "obj/boggie/body.obj";
  // std::string model_filepath = "obj/floor.obj";

  Model model(model_filepath);
  std::cerr << "Loaded " << model.nverts() << " verts, " << model.nfaces()
            << " faces" << std::endl;

  using clock = std::chrono::high_resolution_clock;
  auto last_frame_start = clock::now();

  Input input = {};
  float posX = 0, posY = 0;
  float angleX = 0, angleY = 0.f, angleZ = 0;
  float scaleVal = 1.f;
  float rot_speed = 2.f;
  float move_speed = 1.f;
  float scale_speed = 1.f;
  float camDist = 3.f;
  float zoom_speed = 0.1;
  float fovY = 45.f;
  float focus_speed = 1.f;
  const float radius = 1.03f, margin = 0.5f;
  draw_command dc = {
      .rendering_style = rendering_style::facet_shading,
      .wireframe_color = RED,
      .faceset_shading_color = GREEN,
  };

  while (poll_events(input)) {
    auto now = clock::now();
    float dt = std::chrono::duration<float>(now - last_frame_start).count();
    last_frame_start = now;

    if (input.shift) {
      // Shift held: translate
      if (input.a) posX -= move_speed * dt;
      if (input.d) posX += move_speed * dt;
      if (input.w) posY -= move_speed * dt;
      if (input.s) posY += move_speed * dt;
    } else {
      // No modifier: rotate
      if (input.s) angleX -= rot_speed * dt;
      if (input.w) angleX += rot_speed * dt;
      if (input.a) angleY += rot_speed * dt;
      if (input.d) angleY -= rot_speed * dt;
      if (input.q) angleZ += rot_speed * dt;
      if (input.e) angleZ -= rot_speed * dt;
    }

    // scale: Z/X
    if (input.z) scaleVal -= scale_speed * dt;
    if (input.x) scaleVal += scale_speed * dt;
    if (scaleVal < 0.1f) scaleVal = 0.1f;

    if (input.up) camDist += zoom_speed * dt;
    if (input.down) camDist -= zoom_speed * dt;
    if (input.left) {
      fovY += focus_speed;
      std::cout << fovY << std::endl;
    }
    if (input.right) fovY -= focus_speed;

    camDist = std::max(0.2f, camDist);

    vml::Mat4f camera = lookat(vml::Vec3f(0, 0, camDist), vml::Vec3f(0, 0, 0),
                               vml::Vec3f(0, 1, 0));
    // float far = 5.f, near = 1.8f;
    float near = std::max(0.1f, camDist - margin - radius);
    float far = camDist + margin + radius;
    float aspect = float(vp.xmax - vp.xmin) / float(vp.ymax - vp.ymin);
    dc.transform = perspective(near, far, fovY, aspect) * camera *
                   translate(posX, posY, 0) * rotateX(angleX) *
                   rotateY(angleY) * rotateZ(angleZ) *
                   scale(scaleVal, scaleVal, scaleVal);

    image.clear(BLACK);
    draw(model, image, vp, dc);
    update_display(image);
  }

  destroy_display();
}
