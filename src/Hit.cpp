#include "Hit.h"
#include "Ray.h"

namespace affraytrace
{

HitData::HitData(Ray const &ray, Point3d const &hitPoint, double t, Vec3d const &hitNormal, Material const* material)
    : m_point(hitPoint), m_normal(hitNormal), m_t(t), m_material(material)
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

Material const* HitData::material() const
{
    return m_material;
}



ScatterData::ScatterData(Colour const &attenuation, Ray const &ray)
    : m_attenuation(attenuation), m_ray(ray) {}

Colour ScatterData::attenuation() const
{
    return m_attenuation;
}

Ray ScatterData::ray() const
{
    return m_ray;
}

}
