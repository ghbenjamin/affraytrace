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
      m_focalLength(focalLength),
      m_twister( m_rd() ),
      m_samplesPerPixel(16),
      m_maxDepth( 50 )
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

    double sampleScale = 1.0 / (double) m_samplesPerPixel;

    for (int j = 0; j < m_imageHeight; j++) {

        for (int i = 0; i < m_imageWidth; i++)
        {

            Colour currentPixel{ 0, 0, 0 };

            for ( int k = 0; k < m_samplesPerPixel; k++ )
            {
                Ray sample = get_sampled_ray(i, j);
                currentPixel = currentPixel + ray_colour(sample, 0, scene);
            }

            currentPixel = currentPixel * sampleScale;

            image.setPixel(i, j, currentPixel);
        }
    }

    return image;
}

Ray Camera::get_sampled_ray(const int i, const int j)
{
    Vec3d random_offset = sample_unit_square();

    auto pixel_center =
        m_pixelOrigin
        + ((i + random_offset.x() ) * m_pixel_dX)
        + ((j + random_offset.y()) * m_pixel_dY);

    auto ray_direction = pixel_center - m_origin;

    return { m_origin, ray_direction };
}

Colour Camera::ray_colour(Ray const &ray, int depth, Scene const &scene)
{
    // Temporary hard coded colouring taken from https://raytracing.github.io/books/RayTracingInOneWeekend.html

    if (depth > m_maxDepth)
    {
        return Colour{ 0, 0, 0 };
    }

    if (auto maybe_hit = scene.hit( ray, Interval(0.001f, utils::limits::infinty)); maybe_hit.has_value() )
    {
        Vec3d random_scatter = random_unit_hemisphere_vector(maybe_hit->normal());
        // return 0.5 * (Colour(1, 1, 1) + maybe_hit->normal() );

        return 0.5 * ray_colour( Ray(maybe_hit->point(), random_scatter), depth + 1, scene );
    }

    // Ray hit nothing
    double a = 0.5 * (ray.direction().normalized().y() + 1.0);
    return (1.0 - a) * Colour(1.0, 1.0, 1.0) +
        a * Colour(0.5, 0.7, 1.0);
}

double Camera::random_unit_double()
{
    return m_dist(m_twister);
}

double Camera::random_double(double min, double max)
{
    return min + (max - min) * random_unit_double();
}

Vec3d Camera::sample_unit_square()
{
    // Return a random vector on the centered unit square - betwteen [-0.5,-0.5,0] and [0.5,0.5,0]
    return { random_unit_double() - 0.5, random_unit_double() - 0.5, 0 };
}

Vec3d Camera::random_box_vector()
{
    return { random_double(-1, 1), random_double(-1, 1), random_double(-1, 1) };
}

Vec3d Camera::random_unit_sphere_vector()
{
    while (true)
    {
        Vec3d trial = random_box_vector();

        if (double norm = trial.length2(); norm > 0.000001 && norm <= 1.0 )
        {
            return trial.normalized();
        }
    }
}

Vec3d Camera::random_unit_disc_vector()
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

Vec3d Camera::random_unit_hemisphere_vector(Vec3d const &normal)
{
    Vec3d trial = random_unit_sphere_vector();
    if ( trial.dot(normal) > 0.0 )
    {
        return -trial;
    }
    else
    {
        return trial;
    }
}

}
