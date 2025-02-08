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
    SceneObject(Material* material, Vec3d const& origin);
    virtual ~SceneObject() = default;

    [[nodiscard]]
    virtual std::optional<HitData> hit( Ray const& ray, Interval const& tValid ) const = 0;

protected:
    Material* m_material;
    Vec3d m_origin;
};


class SphereObject : public SceneObject
{
public:
    SphereObject( Material* material, Vec3d const& origin, double radius );

    [[nodiscard]]
    std::optional<HitData> hit( Ray const& ray, Interval const& tValid ) const override;

private:
    double m_radius;
};



}
