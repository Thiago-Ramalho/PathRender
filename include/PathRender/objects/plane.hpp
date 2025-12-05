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
    
    const Color& get_color() const override;
    
    const Point3& get_point() const;

    const Vector3& get_normal() const;

    std::string to_string() const override;

    Point3 get_position() const override;
    
private:
    Point3 m_point;   // Ponto no plano
    Vector3 m_normal; // Normal (normalizada)
    Color m_color;
};

} // namespace PathRender

#endif // PATHRENDER_PLANE_HPP_