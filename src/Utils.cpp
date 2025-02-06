#pragma once

#include <numbers>

#include "Utils.h"

namespace affraytrace
{

double math::radians_to_degrees(double radians)
{
    return radians * 180 / std::numbers::pi;
}

double math::degrees_to_radians(double radians)
{
    return radians * std::numbers::pi / 180;
}

}
