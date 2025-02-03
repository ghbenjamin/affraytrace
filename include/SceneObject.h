#pragma once

#include <optional>

#include "Hit.h"
#include "Vec3.h"

namespace affraytrace
{

class Ray;
class Interval;

class SceneObject
{
public:
    virtual ~SceneObject() = default;

    [[nodiscard]]
    virtual std::optional<HitData> hit( Ray const& ray, Interval const& tValid ) const = 0;
};


class SphereObject : public SceneObject
{
public:
    SphereObject( Vec3d const& origin, double radius, Material* material );

    [[nodiscard]]
    std::optional<HitData> hit( Ray const& ray, Interval const& tValid ) const override;

private:
    Vec3d m_origin;
    double m_radius;
    Material* m_material;
};



}
