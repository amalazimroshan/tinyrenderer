#include "display.h"

#include <SDL2/SDL.h>
#include <SDL_scancode.h>

#include <iostream>

static const char* WINDOW_TITLE = "canvas";
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Texture* texture = nullptr;

bool init_display(int w, int h) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return false;
  }

  window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, w, h, 0);
  if (!window) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                              SDL_TEXTUREACCESS_STREAMING, w, h);
  if (!texture) {
    std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  return true;
}

void update_display(const Framebuffer& fb) {
  SDL_UpdateTexture(texture, nullptr, fb.buffer(), fb.width() * 4);
  SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}

void destroy_display() {
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

bool poll_events(Input& input) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) return false;
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
      return false;
  }

  const Uint8* keys = SDL_GetKeyboardState(nullptr);
  input.w = keys[SDL_SCANCODE_W];
  input.s = keys[SDL_SCANCODE_S];
  input.a = keys[SDL_SCANCODE_A];
  input.d = keys[SDL_SCANCODE_D];
  input.q = keys[SDL_SCANCODE_Q];
  input.e = keys[SDL_SCANCODE_E];
  input.z = keys[SDL_SCANCODE_Z];
  input.x = keys[SDL_SCANCODE_X];
  input.shift = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
  input.up = keys[SDL_SCANCODE_UP];
  input.down = keys[SDL_SCANCODE_DOWN];
  input.left = keys[SDL_SCANCODE_LEFT];
  input.right = keys[SDL_SCANCODE_RIGHT];

  return true;
}
