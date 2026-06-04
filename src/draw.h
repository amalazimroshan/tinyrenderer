#pragma once
#include <vml/vector.hpp>

#include "framebuffer.h"

void line(vml::Vec2i p1, vml::Vec2i p2, Framebuffer& fb, Color color);
vml::Vec2i world2screen(vml::Vec3f v, int width, int height);
