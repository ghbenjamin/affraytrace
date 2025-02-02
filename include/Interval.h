#pragma once

#include "Utils.h"

namespace affraytrace
{

class Interval
{
public:
    Interval();
    Interval(double min, double max);

    [[nodiscard]] double min() const;
    [[nodiscard]] double max() const;

    [[nodiscard]] double size() const;
    [[nodiscard]] bool contains( double val, bool inclusive ) const;

private:
    double m_min;
    double m_max;
};


namespace intervals
{
    static const auto positive_numbers = Interval(0, utils::limits::infinty);
    static const auto empty = Interval(utils::limits::infinty, -utils::limits::infinty);
    static const auto everything = Interval(-utils::limits::infinty, utils::limits::infinty);
}

}