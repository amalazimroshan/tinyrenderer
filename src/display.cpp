#include <display.h>

// const int w = 64;
// const int h = 64;
const char* WINDOW_TITLE = "canvas";

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;

bool init_display(int w, int h) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return false;
  }

  window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, w, h, 0);
  if (window == nullptr) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == nullptr) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGRA32,
                              SDL_TEXTUREACCESS_STREAMING, w, h);
  if (texture == nullptr) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }
  return true;
}

void update_display(const Framebuffer& fb) {
  if (SDL_UpdateTexture(texture, NULL, fb.buffer(), fb.width() * 4) != 0) {
    std::cerr << "SDL_UpdateTexture Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
  }

  if (SDL_RenderCopy(renderer, texture, NULL, NULL) != 0) {
    std::cerr << "SDL_RenderCopy Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
  }

  SDL_RenderPresent(renderer);
}

void destroy_display() {
  SDL_DestroyTexture(texture);
  SDL_RenderClear(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

bool poll_events() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT)
      return false;
    else if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_ESCAPE) return false;
    }
  }
  return true;
}