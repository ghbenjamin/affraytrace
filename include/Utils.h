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
}


