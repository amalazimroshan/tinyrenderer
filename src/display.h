#pragma once
#include "framebuffer.h"

bool init_display(int w, int h);
void update_display(const Framebuffer& fb);
void destroy_display();
bool poll_events();
