#pragma once
#include "framebuffer.h"

struct Input {
  bool w, s;   // +X / -X
  bool a, d;   // +Y / -Y
  bool q, e;   // +Z / -Z
  bool z, x;   // scale down/up
  bool shift;  // held = translate, released = rotate
};

bool init_display(int w, int h);
void update_display(const Framebuffer& fb);
void destroy_display();
bool poll_events(Input& input);
