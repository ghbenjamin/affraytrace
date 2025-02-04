#pragma once

#include "Ray.h"
#include "Utils.h"
#include "Vec3.h"

namespace affraytrace
{

class Ray;
class Material;

class HitData
{
public:
    HitData( Ray const& ray, Point3d const& hitPoint, double t, Vec3d const& hitNormal, Material const* material );

    [[nodiscard]] Point3d point() const;
    [[nodiscard]] Vec3d normal() const;
    [[nodiscard]] double t() const;
    [[nodiscard]] bool isFrontFace() const;
    [[nodiscard]] Material const* material() const;

private:

    // Where was this hit
    Point3d m_point;

    // The unit normal from our hit surface at the hit point
    Vec3d m_normal;

    // The t parameter of the originating ray at the hit
    double m_t;

    // Is this hit against the front or back face of the surface?
    bool m_isFrontFace;

    // What kind of material did we hit?
    Material const* m_material;
};


class ScatterData
{
public:
    ScatterData( Colour const& attenuation, Ray const& ray );

    [[nodiscard]] Colour attenuation() const;
    [[nodiscard]] Ray ray() const;

private:
    Colour m_attenuation;
    Ray m_ray;
};


}
