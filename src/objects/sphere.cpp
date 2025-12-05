#include "PathRender/objects/sphere.hpp"
#include <cmath>

namespace PathRender {

Sphere::Sphere(const Point3& center, float radius, const Color& color)
    : m_center(center), m_radius(radius), m_color(color) {}

bool Sphere::intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const {
    Vector3 oc = ray.origin - m_center;

    float a = ray.direction.dot(ray.direction);
    float half_b = oc.dot(ray.direction);
    float c = oc.dot(oc) - m_radius * m_radius;

    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0f) {
        return false;
    }

    float sqrt_d = std::sqrt(discriminant);

    float root = (-half_b - sqrt_d) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || root > t_max) {
            return false;
        }
    }

    hit.t = root;
    hit.point = ray.at(hit.t);
    hit.color = m_color;
    Vector3 outward_normal = (hit.point - m_center) / m_radius;
    hit.set_face_normal(ray, outward_normal);

    return true;
}

const Color& Sphere::get_color() const { 
    return m_color; 
}
    
const Point3& Sphere::get_center() const { 
    return m_center; 
}

const float& Sphere::get_radius() const { 
    return m_radius; 
}

std::string Sphere::to_string() const {
    return "Sphere(center=" + std::to_string(m_center.x) + "," + std::to_string(m_center.y) + "," + std::to_string(m_center.z) + ", radius=" + std::to_string(m_radius) + ")";
}

Point3 Sphere::get_position() const {
    return m_center;
}

} // namespace PathRender
