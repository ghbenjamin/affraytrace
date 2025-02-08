#pragma once

#include <random>

#include "Vec3.h"
#include "Image.h"
#include "Random.h"

namespace affraytrace
{

// Forward Definitions
class Scene;
class SceneObject;
class Ray;


struct CameraData
{
    int width = 800;
    int height = 600;
    Vec3d origin = Vec3d{0, 0, 0};
    Vec3d target = Vec3d{0, 0, -1};
    double focalLength = 1.0;
    double vertical_fov = 90.;
    int samplesPerPixel = 10;
    int maxScatteringDepth = 50;
};

class Camera
{
public:
    Camera( std::shared_ptr<Random> const& random, CameraData const& data );
    Image render( Scene const& scene );

private:

    // Size of the generated image
    int m_imageWidth;
    int m_imageHeight;

    // Origin and focal length of the camera
    Point3d m_origin;
    Point3d m_target;
    double m_vertical_fov;

    // Antialiasing
    int m_samplesPerPixel;

    // Scattering
    int m_maxDepth;

    // Derived information about the virtual viewport
    Point3d m_pixelOrigin;
    Vec3d m_pixel_dY;
    Vec3d m_pixel_dX;

    // Random state
    std::shared_ptr<Random> m_random;

private:

    // Return a ray directed at a randomly sampled point in the unit square centered on the pixel (i, j)
    Ray get_sampled_ray( int i, int j );

    // Determine the pixel colour for a single ray.
    Colour ray_colour( Ray const& ray, int depth, Scene const& scene );
};

}
