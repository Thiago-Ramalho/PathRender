#include "PathRender/objects/mesh.hpp"
#include <cmath>

namespace PathRender {

bool Mesh::intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const {
    // TODO: implementar
    return true;
}

void Mesh::add_triangle(const Triangle& triangle) {
    m_triangles.push_back(triangle);
}

void Mesh::add_vertex(const Point3& vertex) {
    m_vertices.push_back(vertex);
}

const std::vector<Triangle>& Mesh::get_triangles() const { 
    return m_triangles; 
}

const std::vector<Point3>& Mesh::get_vertices() const { 
    return m_vertices; 
}

void Mesh::set_material(const Material& material) { 
    m_material = material; 
}

const Material& Mesh::get_material() const { 
    return m_material; 
}

void Mesh::set_color(const Color& color) { 
    m_color = color; 
}

const Color& Mesh::get_color() const { 
    return m_color; 
}

void Mesh::set_name(std::string name) { 
    m_name = name; 
}

const std::string& Mesh::get_name() const { 
    return m_name; 
}

std::string Mesh::to_string() const {
    return "Mesh(name=" + m_name + ", num_vertices=" + std::to_string(m_vertices.size()) + ", num_triangles=" + std::to_string(m_triangles.size()) + ")";
}

} // namespace PathRender
