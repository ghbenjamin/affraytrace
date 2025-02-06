#pragma once

#include <limits>

#include "Vec3.h"


namespace affraytrace
{
    // Three-float representation of a colour.
    using Colour = Vec3<float>;

    namespace limits
    {
        constexpr double infinty = std::numeric_limits<double>::max();
    }

    namespace math
    {
        double radians_to_degrees(double radians);
        double degrees_to_radians(double radians);
    }
}


