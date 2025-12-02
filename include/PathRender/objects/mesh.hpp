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
    
    const std::vector<Triangle>& get_triangles() const;
    const std::vector<Point3>& get_vertices() const;
    
    void set_material(const Material& material);
    const Material& get_material() const;
    
    void set_color(const Color& color);
    const Color& get_color() const override;
    
    void set_name(std::string name);
    const std::string& get_name() const;

    std::string print_triangles() const;

    std::string print_vertices() const;

    std::string to_string() const override;
    
private:
    std::string m_name;
    std::vector<Triangle> m_triangles;
    std::vector<Point3> m_vertices;
    Material m_material;
    Color m_color;
};

} // namespace PathRender

#endif // PATHRENDER_MESH_HPP_