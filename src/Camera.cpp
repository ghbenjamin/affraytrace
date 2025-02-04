#include "Camera.h"

#include "Scene.h"
#include "Interval.h"
#include "Material.h"
#include "Ray.h"

namespace affraytrace
{

Camera::Camera( Random& random, CameraData const& data )
    :
        m_imageWidth(data.width),
        m_imageHeight(data.height),
        m_origin(data.origin),
        m_focalLength(data.focalLength),
        m_samplesPerPixel(data.samplesPerPixel),
        m_maxDepth( data.maxScatteringDepth ),
        m_random(random)
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
    Vec3d random_offset = m_random.sample_unit_square();

    auto pixel_center =
        m_pixelOrigin
        + ((i + random_offset.x() ) * m_pixel_dX)
        + ((j + random_offset.y()) * m_pixel_dY);

    auto ray_direction = pixel_center - m_origin;

    return { m_origin, ray_direction };
}

Colour Camera::ray_colour(Ray const &ray, int depth, Scene const &scene)
{
    // At max depth, stop bouncing and return 0. (We ought to be nearly there already assuming that the scene objects
    // have a typical albedo)
    if (depth > m_maxDepth)
    {
        return Colour{ 0, 0, 0 };
    }

    // Interval starts at 0.001 to avoid re-registering the same hit multiple times
    if (auto maybe_hit = scene.hit( ray, Interval(0.001, limits::infinty)); maybe_hit.has_value() )
    {
        if ( auto maybe_scatter = maybe_hit->material()->scatter(m_random, ray, *maybe_hit ); maybe_scatter.has_value() )
        {
            // We hit, and scattered the ray
            return maybe_scatter->attenuation().elementwise_product( ray_colour( maybe_scatter->ray(), depth + 1, scene ) );
        }
        else
        {
            // We hit, but did not scatter. Return black to simulate absorption of the ray
            return Colour{ 0, 0, 0 };
        }
    }

    // Ray hit nothing. Return the colour of the skybox.

    // Temporary hard coded colouring taken from https://raytracing.github.io/books/RayTracingInOneWeekend.html
    // LERP in the y-axis from white to sky blue
    double a = 0.5 * (ray.direction().normalized().y() + 1.0);
    return (1.0 - a) * Colour(1.0, 1.0, 1.0) +
        a * Colour(0.5, 0.7, 1.0);
}

}
