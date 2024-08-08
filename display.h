#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL2/SDL.h>
#include "tgaimage.h"

// Function to create an SDL window and renderer
bool create_window(int width, int height, SDL_Window*& window, SDL_Renderer*& renderer);

// Function to display the TGA image using SDL
void display_tga_image(SDL_Renderer* renderer, TGAImage& image);

#endif // SDL_UTILS_H
