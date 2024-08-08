// graphics.h
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "geometry.h"
#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 0);
const TGAColor green = TGAColor(0, 255, 0, 0);
const TGAColor blue = TGAColor(255, 0, 0, 0);

void line(Vec2i t0, Vec2i t1, TGAImage &image, TGAColor color);
void standard_triangle_fill(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image,
                            TGAColor color);
void scanline_triangle_fill(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image,
                            TGAColor color);

#endif
