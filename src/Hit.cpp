#include "Hit.h"

#include "Ray.h"

namespace affraytrace
{
HitData::HitData(Ray const &ray, Point3d const &hitPoint, double t, Vec3d const &hitNormal)
    : m_point(hitPoint), m_normal(hitNormal), m_t(t)
{
    m_isFrontFace = ray.direction().dot(m_normal) < 0;

    if (!m_isFrontFace)
    {
        m_normal = -m_normal;
    }
}

Point3d HitData::point() const
{
    return m_point;
}

Vec3d HitData::normal() const
{
    return m_normal;
}

double HitData::t() const
{
    return m_t;
}

bool HitData::isFrontFace() const
{
    return m_isFrontFace;
}
}
