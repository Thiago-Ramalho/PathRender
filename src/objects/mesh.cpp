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

} // namespace PathRender
