#include "Material.h"
#include "Random.h"

namespace affraytrace
{

std::optional<ScatterData> Material::scatter(Random& random, Ray const &ray, HitData const &hit_data) const
{
    return std::nullopt;
}

DiffuseMaterial::DiffuseMaterial(Colour const &albedo)
    : m_albedo(albedo) {}

std::optional<ScatterData> DiffuseMaterial::scatter(Random& random, const Ray &ray, const HitData &hit_data) const
{
    Vec3d scatter_direction = hit_data.normal() + random.random_unit_sphere_vector();

    // If our scatter direction is zero, then our scattered ray will have no direction
    // Fall back on the normal in this case
    if ( scatter_direction.almost_zero() )
    {
        scatter_direction = hit_data.normal();
    }

    return ScatterData{ m_albedo, Ray{ hit_data.point(), scatter_direction } };
}

MetalMaterial::MetalMaterial(Colour const &albedo)
    : m_albedo(albedo) {}

std::optional<ScatterData> MetalMaterial::scatter(Random &random, Ray const &ray, HitData const &hit_data) const
{
    Vec3d scatter_direction = ray.direction().reflect( hit_data.normal() );
    return ScatterData{ m_albedo, Ray{ hit_data.point(), scatter_direction } };
}

}
