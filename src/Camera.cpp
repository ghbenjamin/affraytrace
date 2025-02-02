#include "Camera.h"

#include <iso646.h>

#include "Scene.h"
#include "Interval.h"
#include "Ray.h"

namespace affraytrace
{

Camera::Camera(const int width, const int height, Vec3d origin, double focalLength)
    : m_imageWidth(width),
      m_imageHeight(height),
      m_origin(origin),
      m_focalLength(focalLength)

{
    double viewportHeight = 2;
    double viewportWidth = viewportHeight * (static_cast<double>(m_imageWidth)/m_imageHeight);

    // 'axis' vectors for the entire viewport
    Vec3d viewportX = Vec3d(viewportWidth, 0, 0 );
    Vec3d viewportY = Vec3d(0, -viewportHeight, 0 );

    // X/Y vectors from one viewport pixel to the next
    m_pixel_dX = viewportX / m_imageWidth;
    m_pixel_dY = viewportY / m_imageHeight;

    // The top-left origin of the viewport
    Point3d viewportOrigin = m_origin - Vec3d(0, 0, m_focalLength) - (viewportX / 2) - (viewportY / 2);

    // The center of the top-left viewport pixel
    m_pixelOrigin = viewportOrigin + 0.5 * ( m_pixel_dX + m_pixel_dY );
}

Image Camera::render(Scene const &scene)
{
    Image image(m_imageWidth, m_imageHeight);

    for (int j = 0; j < m_imageHeight; j++) {

        for (int i = 0; i < m_imageWidth; i++)
            {
            auto pixel_center = m_pixelOrigin + (i * m_pixel_dX) + (j * m_pixel_dY);
            auto ray_direction = pixel_center - m_origin;

            Ray r(m_origin, ray_direction);

            Colour pixel_color = ray_colour(r, scene);
            image.setPixel(i, j, pixel_color);
        }
    }

    return image;
}

Colour Camera::ray_colour(Ray const &ray, Scene const &scene)
{
    // Temporary hard coded colouring taken from https://raytracing.github.io/books/RayTracingInOneWeekend.html

    if (auto maybe_hit = scene.hit( ray, intervals::positive_numbers ); maybe_hit.has_value() )
    {
        return 0.5 * (Colour(1, 1, 1) + maybe_hit->normal() );
    }

    double a = 0.5*(ray.direction().normalized().y() + 1.0);

    return (1.0 - a) * Colour(1.0, 1.0, 1.0) +
        a * Colour(0.5, 0.7, 1.0);
}
}
