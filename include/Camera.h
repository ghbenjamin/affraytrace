#pragma once

#include <random>

#include "Vec3.h"
#include "Image.h"

namespace affraytrace
{

// Forward Definitions
class Scene;
class SceneObject;
class Ray;


class Camera
{
public:
    Camera(int width, int height, Vec3d origin, double focalLength);
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
    std::random_device m_rd;
    std::mt19937 m_twister;
    std::uniform_real_distribution<> m_dist;

private:

    // Return a ray directed at a randomly sampled point in the unit square centered on the pixel (i, j)
    Ray get_sampled_ray( int i, int j );

    // Temporary hard coded ray -> colour
    Colour ray_colour( Ray const& ray, int depth, Scene const& scene );

    // A random double on [0, 1)
    double random_unit_double();

    // A random double on [min, max)
    double random_double( double min, double max );

    // Random Vec3 between [-0,5,-0.5,0] and [0.5,0.5,0]
    Vec3d sample_unit_square();

    // Random Vec3 between [-1,-1,-1] and [1,1,1]
    Vec3d random_box_vector();

    // Random Vec3 on the surface of the unit sphere
    Vec3d random_unit_sphere_vector();

    // Random Vec3 on the surface of the unit disc with z=0
    Vec3d random_unit_disc_vector();

    // Random Vec3 on the surface of the unit hemisphere as defined by the given normal
    Vec3d random_unit_hemisphere_vector(Vec3d const& normal);

};

}
