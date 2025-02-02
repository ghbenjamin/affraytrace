#pragma once
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

    // Derived information about the virtual viewport
    Point3d m_pixelOrigin;
    Vec3d m_pixel_dY;
    Vec3d m_pixel_dX;

    // Temporary hard coded ray -> colour
    Colour ray_colour( Ray const& ray, Scene const& scene );

};

}
