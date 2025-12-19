#ifndef PATHRENDER_TRIANGLE_HPP_
#define PATHRENDER_TRIANGLE_HPP_

#include "PathRender/objects/objects.hpp"
#include "PathRender/core/point.hpp"
#include "PathRender/core/vector.hpp"
#include <array>

namespace PathRender {

class Triangle : public Object {
public:
    Triangle(const Point3& A, const Point3& B, const Point3& C, const Material& material);

    bool intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const override;
    
    const Color& get_color() const override { return m_material.brdf->color; }

    std::string to_string() const override {
        return "Triangle(A=" + std::to_string(m_vertices[0].x) + "," + std::to_string(m_vertices[0].y) + "," + std::to_string(m_vertices[0].z) +
               ", B=" + std::to_string(m_vertices[1].x) + "," + std::to_string(m_vertices[1].y) + "," + std::to_string(m_vertices[1].z) +
               ", C=" + std::to_string(m_vertices[2].x) + "," + std::to_string(m_vertices[2].y) + "," + std::to_string(m_vertices[2].z) + ")";
    }

    Vector3 get_normal() const;

    Point3 get_position() const override;
    
private:
    std::array<Point3, 3> m_vertices;
};

} // namespace PathRender

#endif // PATHRENDER_TRIANGLE_HPP_