#pragma once
#include <cmath>
#include <vml/matrix.hpp>

vml::Mat4f rotateX(float angle);
vml::Mat4f rotateY(float angle);
vml::Mat4f rotateZ(float angle);
vml::Mat4f translate(float x, float y, float z);
vml::Mat4f scale(float x, float y, float z);
