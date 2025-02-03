#pragma once
#include <random>

#include "Vec3.h"

namespace affraytrace
{

class Random
{
public:
    Random();

    // A random double on [0, 1)
    double random_unit_double();

    // A random double on [min, max)
    double random_double( double min, double max );

    // Random Vec3 between [-0,5,-0.5,0] and [0.5,0.5,0]
    Vec3d sample_unit_square();

    // Random Vec3 between [-1,-1,-1] and [1,1,1]
    Vec3d random_box_vector();

    // Random Vec3 on the surface of the unit sphere
    Vec3d random_unit_sphere_vector();

    // Random Vec3 on the surface of the unit disc with z=0
    Vec3d random_unit_disc_vector();

    // Random Vec3 on the surface of the unit hemisphere as defined by the given normal
    Vec3d random_unit_hemisphere_vector(Vec3d const& normal);

private:
    std::mt19937 m_twister;
    std::uniform_real_distribution<> m_dist;
};

}
