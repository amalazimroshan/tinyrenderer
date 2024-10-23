#include <iostream>

#include "display.h"
#include "geometry.h"
#include "graphics.h"
#include "model.h"
#include "tgaimage.h"

Model* model = NULL;

const int width = 800;
const int height = 800;
const int depth = 225;

Vec3f camera(0, 0, 3);
Vec3f light_dir(0, 0, -1);
Vec3f eye(0, 0, 3);
Vec3f center(0, 0, 0);

Matrix viewport(int x, int y, int w, int h) {
  Matrix m = Matrix::identity(4);
  m[0][3] = x + w / 2.f;
  m[1][3] = y + h / 2.f;
  m[2][3] = depth / 2.f;

  m[0][0] = w / 2.f;
  m[1][1] = h / 2.f;
  m[2][2] = depth / 2.f;
  return m;
}
Matrix lookat(Vec3f eye, Vec3f center, Vec3f up) {
  Vec3f z = (eye - center).normalize();
  Vec3f x = (up ^ z).normalize();
  Vec3f y = (z ^ x).normalize();
  Matrix res = Matrix::identity(4);
  for (int i = 0; i < 3; i++) {
    res[0][i] = x[i];
    res[1][i] = y[i];
    res[2][i] = z[i];
    res[i][3] = -center[i];
  }
  return res;
}
int main() {
  TGAImage image(width, height, TGAImage::RGB);
  model = new Model("obj/african_head.obj");
  TGAImage texture;
  texture.read_tga_file("./african_head_diffuse.tga");
  texture.flip_vertically();

  Matrix ModelView = lookat(eye, center, Vec3f(0, 1, 0));
  Matrix ViewPort =
      viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
  Matrix Projection = Matrix::identity(4);
  Projection[3][2] = -1.f / (eye - center).norm();

  float* zbuffer = new float[width * height];
  for (int i = width * height; i--;
       zbuffer[i] = -std::numeric_limits<float>::max());
  for (int i = 0; i < model->nfaces(); i++) {
    std::vector<int> face = model->face(i);
    std::vector<int> uv_indices = model->uv_indices(i);

    Vec3f screen_coords[3], world_coords[3];
    Vec2f uv_coords[3];
    for (int j = 0; j < 3; j++) {
      // screen_coords[j] = world2screen(model->vert(face[j]));
      screen_coords[j] = Vec3f(ViewPort * Projection * ModelView *
                               Matrix(model->vert(face[j])));
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