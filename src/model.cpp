#include "model.h"

#include <cstdio>
#include <fstream>

Model::Model(const std::string& filename) {
  std::ifstream in(filename);
  std::string line;
  while (std::getline(in, line)) {
    if (line.size() < 2) continue;
    if (line[0] == 'v' && line[1] == ' ') {
      vml::Vec3f v;
      sscanf(line.c_str() + 2, "%f %f %f", &v.x, &v.y, &v.z);
      verts.push_back(v);
    } else if (line[0] == 'f' && line[1] == ' ') {
      std::vector<int> face;
      const char* p = line.c_str() + 2;
      int idx;
      while (sscanf(p, "%d", &idx) == 1) {
        face.push_back(idx - 1);
        while (*p && *p != ' ') p++;
        while (*p == ' ') p++;
      }
      faces.push_back(face);
    }
  }
}

int Model::nverts() const { return verts.size(); }
int Model::nfaces() const { return faces.size(); }
