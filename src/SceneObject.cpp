#include "SceneObject.h"

#include "Interval.h"
#include "Ray.h"

namespace affraytrace
{
SceneObject::SceneObject(Material *material, Vec3d const &origin)
    : m_origin(origin), m_material(material) {}

SphereObject::SphereObject(Material* material, Vec3d const& origin, double radius )
    : SceneObject(material, origin), m_radius(radius) {}

std::optional<HitData> SphereObject::hit(Ray const &ray, Interval const& tValid) const
{
    // Sphere origin Cx,Cy,Cz, Ray origin Qx,Qy,Qz.

    Vec3d QtoC = m_origin - ray.origin();

    // A point P on a sphere w/ radius r obeys:
    // (C - P)^2 = r^2

    // A point P on the ray w/ direction D & parameter t obeys:
    // P = Q + tD

    // Solving the above yields a quadratic in t. Plugging into the quadratic formula yields
    // a = D^2
    // b = -2D . QtoC
    // c = QtoC^2 - r^2

    double qparam_a = ray.direction().length2();
    double qparam_b = - 2 * ray.direction().dot(QtoC);
    double qparam_c = QtoC.length2() - m_radius * m_radius;

    double discriminant = qparam_b * qparam_b - 4 * qparam_a * qparam_c;

    if ( discriminant < 0 )
    {
        return std::nullopt;
    }


    // First, try positive root: -b + sqrt(discriminant) // 2a
    double root = (-qparam_b - std::sqrt(discriminant)) / (2 * qparam_a);

    if ( !tValid.contains(root, false) )
    {
        // This root isn't valid - try the other one.
        root = (-qparam_b - std::sqrt(discriminant)) / (2 * qparam_a);
        if ( !tValid.contains(root, false) )
        {
            // This one isn't valid either - we don't have a valid hit.
            return std::nullopt;
        }
    }

    // If we've gotten this far then we've found a value of t which is a valid hit
    // Return Q + tD, and some assorted data about the hit

    Point3d hitPoint = ray.at(root);

    HitData hitData( ray, hitPoint, root, (hitPoint - m_origin) / m_radius, m_material );
    return hitData;
}

}
