#include "PathRender/objects/plane.hpp"
#include <cmath>

namespace PathRender {

Plane::Plane(const Point3& point, const Vector3& normal, const Color& color)
    : m_point(point), m_normal(normal.normalized()), m_color(color) {}

bool Plane::intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const {
    float denom = ray.direction.dot(m_normal);
    if (std::abs(denom) < 1e-6f) {
        return false; // Ray parallel to plane
    }

    float t = (m_point - ray.origin).dot(m_normal) / denom;
    if (t < t_min || t > t_max) {
        return false;
    }

    hit.t = t;
    hit.point = ray.at(t);
    hit.color = m_color;
    hit.set_face_normal(ray, m_normal);

    return true;
}

} // namespace PathRender
