#pragma once

#include <type_traits>
#include <cmath>

namespace affraytrace
{

template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template <typename T> requires arithmetic<T>
class Vec3
{
public:
  Vec3() : m_x{0}, m_y{0}, m_z{0} {}
  Vec3(T x, T y, T z) : m_x{x}, m_y{y}, m_z{z} {};

  T x() const { return m_x; }
  T y() const { return m_y; }
  T z() const { return m_z; }

  Vec3<T> operator-() const { return Vec3<T>(-m_x, -m_y, -m_z); }


  T length2() const
  {
    return m_x * m_x + m_y * m_y + m_z * m_z;
  }

  T length() const
  {
    return std::sqrt(length2());
  }


  T dot(const Vec3<T> &other) const
  {
    return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
  }

  Vec3<T> cross(const Vec3<T> &other) const
  {
    return Vec3<T>(
        m_y * other.m_z - m_z * other.m_y,
        -m_x * other.m_z + m_z * other.m_x,
        m_x * other.m_y - m_y * other.m_x
    );
  }

  Vec3<T> normalized() const
  {
    return *this / length();
  }

private:
  T m_x;
  T m_y;
  T m_z;


};


using Vec3d = Vec3<double>;
using Point3d = Vec3<double>;



template <typename T, typename U> requires arithmetic<U>
Vec3<T> operator*(U v, Vec3<T> const& vec)
{
  return Vec3<T>(vec.x() * v, vec.y() * v, vec.z() * v);
}

template <typename T, typename U> requires arithmetic<U>
Vec3<T> operator*(Vec3<T> const& vec, U v)
{
  return v * vec;
}

template <typename T, typename U> requires arithmetic<U>
Vec3<T> operator/(Vec3<T> const& vec, U v)
{
  return Vec3<T>(vec.x() / v, vec.y() / v, vec.z() / v);
}

template <typename T> requires arithmetic<T>
Vec3<T> operator+(Vec3<T> const& vec1, Vec3<T> const& vec2)
{
  return Vec3<T>( vec1.x() + vec2.x(), vec1.y() + vec2.y(), vec1.z() + vec2.z() );
}

template <typename T, typename U> requires arithmetic<T> && arithmetic<U>
Vec3<T> operator+(Vec3<T> const& vec1, Vec3<U> const& vec2)
{
  return Vec3<T>( vec1.x() + (T)vec2.x(), vec1.y() + (T)vec2.y(), vec1.z() + (T)vec2.z() );
}

template <typename T> requires arithmetic<T>
Vec3<T> operator-(Vec3<T> const& vec1, Vec3<T> const& vec2)
{
  return Vec3<T>( vec1.x() - vec2.x(), vec1.y() - vec2.y(), vec1.z() - vec2.z() );
}


}