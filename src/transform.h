#pragma once
#include <vml/matrix.hpp>

vml::Mat4f rotateX(float angle);
vml::Mat4f rotateY(float angle);
vml::Mat4f rotateZ(float angle);
vml::Mat4f translate(float x, float y, float z);
vml::Mat4f scale(float x, float y, float z);
vml::Mat4f perspective(float near, float far, float fovY, float aspect);
vml::Mat4f lookat(const vml::Vec3f& eye, const vml::Vec3f& target,
                  const vml::Vec3f& upDir);
