#pragma once
#include <cstdint>
#include <cstdlib>

struct Color {
  std::uint8_t r, g, b, a;
};

inline Color colorFromIndex(int i) {
  unsigned h = static_cast<unsigned>(i) * 2654435761u;
  return {static_cast<std::uint8_t>(h),
          static_cast<std::uint8_t>(h >> 8),
          static_cast<std::uint8_t>(h >> 16), 255};
}

constexpr Color RED = {255, 0, 0, 255};
constexpr Color GREEN = {0, 255, 0, 255};
constexpr Color BLUE = {0, 0, 255, 255};
constexpr Color BLACK = {0, 0, 0, 255};
constexpr Color WHITE = {255, 255, 255, 255};
