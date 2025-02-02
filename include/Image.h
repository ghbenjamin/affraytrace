#pragma once

#include <vector>
#include <string>

#include "Colour.h"

namespace affraytrace
{

class Image
{

public:
  Image( int width, int height );
  void save(std::string const& filename);

  void setPixel( int x, int y, Colour const& colour );

private:
  const int m_width;
  const int m_height;
  const int m_rowBytes;

  std::vector<float> m_data;

  // Number of bytes per pixel. RGB -> 3
  static constexpr int STRIDE = 3;
};

}
