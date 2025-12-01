#ifndef PATHRENDER_PLANE_HPP_
#define PATHRENDER_PLANE_HPP_

#include "PathRender/objects/objects.hpp"
#include "PathRender/core/point.hpp"
#include "PathRender/core/vector.hpp"
#include "PathRender/core/color.hpp"

namespace PathRender {

/**
 * @class Plane
 * @brief Representa um plano infinito no espaço 3D
 */
class Plane : public Object {
public:
    /**
     * @brief Constrói um plano
     * @param point Um ponto no plano
     * @param normal Vetor normal ao plano (será normalizado)
     * @param color Cor do plano
     */
    Plane(const Point3& point, const Vector3& normal, const Color& color);
    
    /**
     * @brief Testa interseção entre um raio e o plano
     * 
     * Usa a equação do plano:
     * (P - P0) · N = 0
     * onde P = origem + t * direção
     */
    bool intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const override;
    
    Color get_color() const override { return m_color; }
    
    Point3 point() const { return m_point; }
    Vector3 normal() const { return m_normal; }

    std::string to_string() const override {
        return "Plane(point=" + std::to_string(m_point.x) + "," + std::to_string(m_point.y) + "," + std::to_string(m_point.z) +
               ", normal=" + std::to_string(m_normal.x) + "," + std::to_string(m_normal.y) + "," + std::to_string(m_normal.z) + ")";
    }
    
private:
    Point3 m_point;   // Ponto no plano
    Vector3 m_normal; // Normal (normalizada)
    Color m_color;
};

} // namespace PathRender

#endif // PATHRENDER_PLANE_HPP_