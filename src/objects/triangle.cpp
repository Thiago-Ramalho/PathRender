#include "PathRender/objects/triangle.hpp"
#include <cmath>

namespace PathRender {

Triangle::Triangle(const Point3& A, const Point3& B, const Point3& C, const Color& color)
    : m_vertices({A, B, C}), m_color(color) {}

bool Triangle::intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const {
    return true;
}

} // namespace PathRender
