#include "transform.h"

#include <cmath>

#include "vml/matrix.hpp"
#include "vml/vector.hpp"

vml::Mat4f rotateX(float angle) {
  vml::Mat4f m = vml::Mat4f::identity();
  m(1, 1) = cos(angle);
  m(1, 2) = -sin(angle);
  m(2, 1) = sin(angle);
  m(2, 2) = cos(angle);
  return m;
}

vml::Mat4f rotateY(float angle) {
  vml::Mat4f m = vml::Mat4f::identity();
  m(0, 0) = cos(angle);
  m(0, 2) = sin(angle);
  m(2, 0) = -sin(angle);
  m(2, 2) = cos(angle);
  return m;
}

vml::Mat4f rotateZ(float angle) {
  vml::Mat4f m = vml::Mat4f::identity();
  m(0, 0) = cos(angle);
  m(0, 1) = -sin(angle);
  m(1, 0) = sin(angle);
  m(1, 1) = cos(angle);
  return m;
}

vml::Mat4f translate(float x, float y, float z) {
  vml::Mat4f m = vml::Mat4f::identity();
  m(0, 3) = x;
  m(1, 3) = y;
  m(2, 3) = z;
  return m;
}

vml::Mat4f scale(float x, float y, float z) {
  vml::Mat4f m = vml::Mat4f::identity();
  m(0, 0) = x;
  m(1, 1) = y;
  m(2, 2) = z;
  return m;
}

vml::Mat4f perspective(float near, float far, float fovY, float aspect) {
  const float DEG2RAD = M_PI / 180.f;
  float top = near * std::tan(fovY / 2.f * DEG2RAD);
  float right = top * aspect;
  vml::Mat4f m = vml::Mat4f::identity();
  m(0, 0) = near / right;
  m(1, 1) = near / top;
  m(2, 2) = (near + far) / (near - far);
  m(2, 3) = 2 * near * far / (near - far);
  m(3, 2) = -1.f;
  m(3, 3) = 0.f;
  return m;
}

vml::Mat4f lookat(const vml::Vec3f& eye, const vml::Vec3f& target,
                  const vml::Vec3f& upDir) {
  vml::Vec3f f = vml::normalize(eye - target);
  vml::Vec3f l = vml::normalize(vml::cross(upDir, f));
  vml::Vec3f u = vml::cross(f, l);

  vml::Mat4f m = vml::Mat4f::identity();
  m(0, 0) = l.x;
  m(0, 1) = l.y;
  m(0, 2) = l.z;

  m(1, 0) = u.x;
  m(1, 1) = u.y;
  m(1, 2) = u.z;

  m(2, 0) = f.x;
  m(2, 1) = f.y;
  m(2, 2) = f.z;

  m(0, 3) = -l.x * eye.x - l.y * eye.y - l.z * eye.z;
  m(1, 3) = -u.x * eye.x - u.y * eye.y - u.z * eye.z;
  m(2, 3) = -f.x * eye.x - f.y * eye.y - f.z * eye.z;
  return m;
}
