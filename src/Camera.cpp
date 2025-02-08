#include "Camera.h"

#include <iostream>
#include <chrono>

#include "Scene.h"
#include "Interval.h"
#include "Material.h"
#include "Ray.h"

namespace affraytrace
{

Camera::Camera( std::shared_ptr<Random> const& random, CameraData const& data )
    :
        m_imageWidth(data.width),
        m_imageHeight(data.height),
        m_origin(data.origin),
        m_target(data.target),
        m_samplesPerPixel(data.samplesPerPixel),
        m_maxDepth( data.maxScatteringDepth ),
        m_random(random),
        m_vertical_fov(data.vertical_fov)
{

    Vec3d camera_up = {0, 1, 0};

    double focal_length = (m_origin - m_target).length();

    double vfov_rads = math::degrees_to_radians(m_vertical_fov);
    double viewportHeight = 2 * focal_length * std::tan(vfov_rads / 2);
    double viewportWidth = viewportHeight * (static_cast<double>(m_imageWidth)/m_imageHeight);

    Vec3d camera_coords_w = (m_origin - m_target).normalized();
    Vec3d camera_coords_u = camera_up.cross(camera_coords_w).normalized();
    Vec3d camera_coords_v = camera_coords_w.cross(camera_coords_u).normalized();


    // 'axis' vectors for the entire viewport
    Vec3d viewportX = viewportWidth * camera_coords_u;
    Vec3d viewportY = viewportHeight * -camera_coords_v;

    // X/Y vectors from one viewport pixel to the next
    m_pixel_dX = viewportX / m_imageWidth;
    m_pixel_dY = viewportY / m_imageHeight;

    // The top-left origin of the viewport
    Point3d viewportOrigin = m_origin - (focal_length * camera_coords_w) - (viewportX / 2) - (viewportY / 2);

    // The center of the top-left viewport pixel
    m_pixelOrigin = viewportOrigin + 0.5 * ( m_pixel_dX + m_pixel_dY );
}

Image Camera::render(Scene const &scene)
{
    Image image(m_imageWidth, m_imageHeight);

    double sampleScale = 1.0 / (double) m_samplesPerPixel;

    log_string( "Starting render..." );

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int j = 0; j < m_imageHeight; j++) {

        std::clog << std::format("Rendering line {:d} of {:d}", j+1, m_imageHeight) << "\r";
        std::clog.flush();

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


    auto end_time = std::chrono::high_resolution_clock::now();
    double elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    double elapsed_nanoseconds = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
    double elapsed_micros_per_pixel = elapsed_nanoseconds / (m_imageHeight * m_imageWidth);

    log_string( "\nDone.\n" );
    log_string( "Total elapsed time: {:.2f}s", elapsed_milliseconds / 1000 );
    log_string( "Elapsed time per pixel: {:.2f}us", elapsed_micros_per_pixel );


    return image;
}

Ray Camera::get_sampled_ray(const int i, const int j)
{
    Vec3d random_offset = m_random->sample_unit_square();

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
