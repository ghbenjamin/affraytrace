#include "Scene.h"
#include "Interval.h"


namespace affraytrace
{

std::optional<HitData> Scene::hit(Ray const &ray, Interval const& tValid) const
{
    std::optional<HitData> lastHit;

    double closestHit = tValid.max();

    for ( auto const& object : m_sceneObjects )
    {
        if (std::optional<HitData> currentHit = object->hit(ray, Interval(tValid.min(), closestHit)); currentHit.has_value())
        {
            lastHit = currentHit;
            closestHit = currentHit->t();
        }
    }

    return lastHit;
}

}
