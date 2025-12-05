#include "PathRender/objects/mesh.hpp"
#include <cmath>

namespace PathRender {

bool Mesh::intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const {
    bool intersected = false;
    float closest_so_far = t_max;
    for (const Triangle& triangle : m_triangles) {
        HitRecord temp_hit;
        if (triangle.intersect(ray, t_min, closest_so_far, temp_hit)) {
            hit = temp_hit;
            hit.color = m_color; 
            intersected = true;
            closest_so_far = hit.t;
        }
    }
    return intersected;
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

std::string Mesh::print_triangles() const {
    std::string result;
    for (const auto& triangle : m_triangles) {
        result += triangle.to_string() + "\n";
    }
    return result;
}

std::string Mesh::print_vertices() const {
    std::string result;
    for (const auto& vertex : m_vertices) {
        result += vertex.to_string() + "\n";
    }
    return result;
}

std::string Mesh::to_string() const {
    return "Mesh(name=" + m_name + ", color=" + m_color.to_string() + "\n" + print_vertices() + "\n" + print_triangles() + ")";
}

Point3 Mesh::get_position() const {
    if (m_vertices.empty()) {
        return Point3(0.0f, 0.0f, 0.0f);
    }
    
    Vector3 sum(0.0f, 0.0f, 0.0f);
    for (auto vertex : m_vertices) {
        sum = sum + Vector3(vertex.x, vertex.y, vertex.z);
    }
    return sum / static_cast<float>(m_vertices.size());
}

} // namespace PathRender
