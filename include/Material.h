#pragma once
#include <optional>

#include "Colour.h"
#include "Hit.h"

namespace affraytrace
{

class Random;

class Material
{
public:
    virtual ~Material() = default;
    virtual std::optional<ScatterData> scatter( Random& random, Ray const& ray, HitData const& hit_data ) const;
};


class DiffuseMaterial : public Material
{
public:
    explicit DiffuseMaterial( Colour const& albedo );
    std::optional<ScatterData> scatter( Random& random, Ray const& ray, HitData const& hit_data ) const override;

private:
    Colour m_albedo;
};

class MetalMaterial : public Material
{
public:
    explicit MetalMaterial( Colour const& albedo );
    std::optional<ScatterData> scatter( Random& random, Ray const& ray, HitData const& hit_data ) const override;

private:
    Colour m_albedo;
};


}
