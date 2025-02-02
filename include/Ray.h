#pragma once

#include "Vec3.h"

namespace affraytrace
{

class Ray
{
public:

  Ray( Vec3d const& origin, Vec3d const& direction ) : m_origin(origin), m_direction(direction) {}

  [[nodiscard]] Vec3d origin() const;
  [[nodiscard]] Vec3d direction() const;

  [[nodiscard]] Vec3d at( double distance ) const;

private:
  Vec3d m_origin;
  Vec3d m_direction;

};

}