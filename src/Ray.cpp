#include "Ray.h"

namespace affraytrace
{
Vec3d Ray::origin() const
{
    return m_origin;
}

Vec3d Ray::direction() const
{
    return m_direction;
}

Vec3d Ray::at(double distance) const
{
    return m_origin + m_direction * distance;
}

}
