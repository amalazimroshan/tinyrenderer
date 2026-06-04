#pragma once
#include <string>
#include <vector>
#include <vml/vector.hpp>

struct Model {
  std::vector<vml::Vec3f> verts;
  std::vector<std::vector<int>> faces;

  Model(const std::string& filename);
  int nverts() const;
  int nfaces() const;
};
