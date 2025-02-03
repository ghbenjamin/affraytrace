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
    Vec3d origin = Vec3d{0, 0.0, 1.5};
    double focalLength = 1.0;
    int samplesPerPixel = 50;
    int maxScatteringDepth = 50;
};

class Camera
{
public:
    Camera( Random& random, CameraData const& data );
    Image render( Scene const& scene );

private:

    // Size of the generated image
    int m_imageWidth;
    int m_imageHeight;

    // Origin and focal length of the camera
    Point3d m_origin;
    double m_focalLength;

    // Antialiasing
    int m_samplesPerPixel;

    // Scattering
    int m_maxDepth;

    // Derived information about the virtual viewport
    Point3d m_pixelOrigin;
    Vec3d m_pixel_dY;
    Vec3d m_pixel_dX;

    // Random state
    Random& m_random;

private:

    // Return a ray directed at a randomly sampled point in the unit square centered on the pixel (i, j)
    Ray get_sampled_ray( int i, int j );

    // Temporary hard coded ray -> colour
    Colour ray_colour( Ray const& ray, int depth, Scene const& scene );



};

}
