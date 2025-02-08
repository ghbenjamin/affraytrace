#pragma once

#include <format>
#include <iostream>
#include <limits>
#include <string_view>

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


    template <typename... Args>
    void log_string( std::string const& format, Args&& ...args )
    {
        std::clog << std::vformat(format, std::make_format_args(std::forward<Args>(args)...)) << std::endl;
    }


    struct IOException final : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    struct FormatException final : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };
}


