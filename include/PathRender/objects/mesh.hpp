#ifndef F83AFB0C_DC0F_4E79_9FAF_BBE8D6B1502E
#define F83AFB0C_DC0F_4E79_9FAF_BBE8D6B1502E
#ifndef PATHRENDER_MESH_HPP_
#define PATHRENDER_MESH_HPP_

#include "PathRender/objects/objects.hpp"
#include "PathRender/core/material.hpp"
#include "PathRender/core/point.hpp"
#include "PathRender/core/color.hpp"
#include "PathRender/objects/triangle.hpp"
#include <vector>
#include <string>
#include <iostream>

namespace PathRender {

class Mesh : public Object {
public:
    Mesh() = default;

    bool intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const override;
    void add_triangle(const Triangle& triangle);
    void add_vertex(const Point3& vertex);
    
    std::vector<Triangle> get_triangles() const { return m_triangles; }
    std::vector<Point3> get_vertices() const { return m_vertices; }
    
    void set_material(const Material& material) { m_material = material; }
    Material get_material() const { return m_material; }
    
    void set_color(const Color& color) { m_color = color; }
    Color get_color() const override { return m_color; }
    
    void set_name(std::string name) {
        m_name = name;
    }
    std::string get_name() const { return m_name; }

    std::string to_string() const override {
        return "Mesh(name=" + m_name + ", num_vertices=" + std::to_string(m_vertices.size()) + ", num_triangles=" + std::to_string(m_triangles.size()) + ")";
    }
    
private:
    std::string m_name;
    std::vector<Triangle> m_triangles;
    std::vector<Point3> m_vertices;
    Material m_material;
    Color m_color;
};

} // namespace PathRender

#endif // PATHRENDER_MESH_HPP_

#endif /* F83AFB0C_DC0F_4E79_9FAF_BBE8D6B1502E */
