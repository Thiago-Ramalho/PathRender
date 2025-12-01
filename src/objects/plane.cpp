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

const Color& Plane::get_color() const { 
    return m_color; 
}
    
const Point3& Plane::get_point() const { 
    return m_point; 
}

const Vector3& Plane::get_normal() const { 
    return m_normal; 
}

std::string Plane::to_string() const {
    return "Plane(point=" + std::to_string(m_point.x) + "," + std::to_string(m_point.y) + "," + std::to_string(m_point.z) +
            ", normal=" + std::to_string(m_normal.x) + "," + std::to_string(m_normal.y) + "," + std::to_string(m_normal.z) + ")";
}

} // namespace PathRender
