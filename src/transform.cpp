#include "transform.h"

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
