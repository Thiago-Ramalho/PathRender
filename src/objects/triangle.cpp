#include "PathRender/objects/triangle.hpp"
#include <cmath>

namespace PathRender {

Triangle::Triangle(const Point3& A, const Point3& B, const Point3& C, const Material& material)
    : m_vertices({A, B, C}) { m_material = material; }

Vector3 Triangle::get_normal() const {
    Vector3 edge1 = m_vertices[1] - m_vertices[0];
    Vector3 edge2 = m_vertices[2] - m_vertices[0];
    return edge1.cross(edge2).normalized();
}

// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool Triangle::intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const {
    const float epsilon = t_min;
    Point3 a = m_vertices[0], 
           b = m_vertices[1], 
           c = m_vertices[2];

    Vector3 edge1 = b - a;
    Vector3 edge2 = c - a;
    Vector3 ray_cross_e2 = ray.direction.cross(edge2);
    float det = edge1.dot(ray_cross_e2);

    if (det > -epsilon && det < epsilon) {
        return false;    // This ray is parallel to this triangle.
    }

    float inv_det = 1.0 / det;
    Vector3 s = ray.origin - a;
    float u = inv_det * (s.dot(ray_cross_e2));

    if ((u < 0 && std::abs(u) > epsilon) || (u > 1 && std::abs(u-1) > epsilon)) {
        return false;
    }

    Vector3 s_cross_e1 = s.cross(edge1);
    float v = inv_det * (ray.direction.dot(s_cross_e1));

    if ((v < 0 && std::abs(v) > epsilon) || (u + v > 1 && std::abs(u + v - 1) > epsilon)) {
        return false;
    }

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * edge2.dot(s_cross_e1);

    if (t > epsilon && t < t_max) // ray intersection
    {
        hit.t = t;
        hit.point = ray.at(t);
        Vector3 outward_normal = this->get_normal();
        hit.set_face_normal(ray, outward_normal);
        return  true; 
    }
    return false;
}

Point3 Triangle::get_position() const {
    auto&& v = m_vertices;
    Vector3 a = Vector3(v[0].x, v[0].y, v[0].z), 
            b = Vector3(v[1].x, v[1].y, v[1].z), 
            c = Vector3(v[2].x, v[2].y, v[2].z);
    return (a + b + c) / 3.0;
}

} // namespace PathRender
