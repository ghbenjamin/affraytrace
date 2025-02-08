#include "Random.h"

#include <iostream>
#include <ostream>

namespace affraytrace
{

Random::Random()
    :m_twister( std::random_device{}() ) {}

double Random::random_unit_double()
{
    return m_dist(m_twister);
}

double Random::random_double(double min, double max)
{

    return min + (max - min) * random_unit_double();
}

Vec3d Random::sample_unit_square()
{
    // Return a random vector on the centered unit square - betwteen [-0.5,-0.5,0] and [0.5,0.5,0]
    return { random_unit_double() - 0.5, random_unit_double() - 0.5, 0 };
}

Vec3d Random::random_box_vector()
{
    return { random_double(-1, 1), random_double(-1, 1), random_double(-1, 1) };
}

Vec3d Random::random_unit_sphere_vector()
{
    while (true)
    {
        Vec3d trial = random_box_vector();

        if (double norm = trial.length2(); norm > 1e-160 && norm <= 1.0 )
        {
            return trial.normalized();
        }
    }
}

Vec3d Random::random_unit_disc_vector()
{
    while (true)
    {
        Vec3d trial = random_box_vector();

        if (double norm = trial.x() * trial.x() + trial.y() * trial.y() ; norm > 0.000001 && norm <= 1.0 )
        {
            double sqrt_norm = std::sqrt(norm);
            return Vec3d{ trial.x() / sqrt_norm, trial.y() / sqrt_norm, 0 };
        }
    }
}

Vec3d Random::random_unit_hemisphere_vector(Vec3d const &normal)
{
    Vec3d trial = random_unit_sphere_vector();
    if ( trial.dot(normal) > 0.0 )
    {
        return trial;
    }
    else
    {
        return -trial;
    }
}

}
