#include "Interval.h"

namespace affraytrace
{

// Unbounded interval contains nothing
Interval::Interval()
    : m_min(limits::infinty), m_max(-limits::infinty) {}

Interval::Interval(const double min, const double max)
    : m_min(min), m_max(max) {}

double Interval::min() const
{
    return m_min;
}

double Interval::max() const
{
    return m_max;
}

double Interval::size() const
{
    return m_max - m_min;
}

bool Interval::contains(const double val, const bool inclusive) const
{
    if (inclusive)
    {
        return val >= m_min && val <= m_max;
    }
    else
    {
        return val > m_min && val < m_max;
    }
}

}
