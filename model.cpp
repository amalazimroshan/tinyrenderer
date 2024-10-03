#include "model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Model::Model(const char *filename) : verts_(), faces_() {
  std::ifstream in;
  in.open(filename, std::ifstream::in);
  if (in.fail()) return;
  std::string line;
  while (!in.eof()) {
    std::getline(in, line);
    std::istringstream iss(line.c_str());
    char trash;
    if (!line.compare(0, 2, "v ")) {
      iss >> trash;
      Vec3f v;
      for (int i = 0; i < 3; i++) iss >> v[i];
      verts_.push_back(v);
    } else if (!line.compare(0, 3, "vt ")) {
      iss >> trash >> trash;
      Vec2f uv;
      for (int i = 0; i < 2; i++) iss >> uv[i];
      uv_.push_back(uv);
    } else if (!line.compare(0, 2, "f ")) {
      std::vector<int> f;
      std::vector<int> uv_idx;
      int itrash, idx, uv_i;
      iss >> trash;
      while (iss >> idx >> trash >> uv_i >> trash >> itrash) {
        idx--;  // in wavefront obj all indices start at 1, not zero
        uv_i--;
        f.push_back(idx);
        uv_idx.push_back(uv_i);
      }
      faces_.push_back(f);
      uv_indices_.push_back(uv_idx);
    }
  }
  std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# "
            << uv_.size() << std::endl;
}

Model::~Model() {}

int Model::nverts() { return (int)verts_.size(); }

int Model::nfaces() { return (int)faces_.size(); }

std::vector<int> Model::face(int idx) { return faces_[idx]; }

Vec3f Model::vert(int i) { return verts_[i]; }

Vec2f Model::uv(int i) { return uv_[i]; }

std::vector<int> Model::uv_indices(int idx) { return uv_indices_[idx]; }
