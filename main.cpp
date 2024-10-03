#include <iostream>

#include "display.h"
#include "geometry.h"
#include "graphics.h"
#include "model.h"
#include "tgaimage.h"

Model* model = NULL;
const int width = 800;
const int height = 800;
Vec3f world2screen(Vec3f v) {
  return Vec3f(int((v.x + 1.) * width / 2. + .5),
               int((v.y + 1.) * height / 2. + .5), v.z);
}
int main(int argc, char* args[]) {
  TGAImage image(width, height, TGAImage::RGB);
  model = new Model("obj/african_head.obj");
  TGAImage texture;
  texture.read_tga_file("./african_head_diffuse.tga");
  texture.flip_vertically();

  Vec3f light_dir(0, 0, -1);
  float* zbuffer = new float[width * height];
  for (int i = width * height; i--;
       zbuffer[i] = -std::numeric_limits<float>::max());
  for (int i = 0; i < model->nfaces(); i++) {
    std::vector<int> face = model->face(i);
    std::vector<int> uv_indices = model->uv_indices(i);

    Vec3f screen_coords[3], world_coords[3];
    Vec2f uv_coords[3];
    for (int j = 0; j < 3; j++) {
      screen_coords[j] = world2screen(model->vert(face[j]));
      world_coords[j] = model->vert(face[j]);
      uv_coords[j] = model->uv(uv_indices[j]);
    }
    // Vec3f n = cross((world_coords[2] - world_coords[1]),
    //                 (world_coords[1] - world_coords[0]));
    // n.normalize();
    // float intensity = n * light_dir;
    // if (intensity > 0)
    BarycentricScanline_triangle_fill(screen_coords, uv_coords, zbuffer, image,
                                      texture);
  }

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