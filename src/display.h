#pragma once
#include <SDL2/SDL.h>
#include <framebuffer.h>

#include <iostream>

bool init_display(int w, int h);
void update_display(const Framebuffer& fb);
void destroy_display();
bool poll_events();