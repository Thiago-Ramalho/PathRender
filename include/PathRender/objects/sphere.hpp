#ifndef PATHRENDER_SPHERE_HPP_
#define PATHRENDER_SPHERE_HPP_

#include "PathRender/objects/objects.hpp"
#include "PathRender/core/point.hpp"
#include "PathRender/core/color.hpp"

namespace PathRender {

/**
 * @class Sphere
 * @brief Representa uma esfera no espaço 3D
 */
class Sphere : public Object {
public:
    /**
     * @brief Constrói uma esfera
     * @param center Centro da esfera
     * @param radius Raio da esfera
     * @param color Cor da esfera
     */
    Sphere(const Point3& center, float radius, const Material& material);
    
    /**
     * @brief Testa interseção entre um raio e a esfera
     * 
     * Usa a fórmula quadrática para resolver:
     * (P - C) · (P - C) = r²
     * onde P = origem + t * direção
     */
    bool intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const override;
    
    const Color& get_color() const override;
    
    const Point3& get_center() const;
    const float& get_radius() const;

    std::string to_string() const override;

    Point3 get_position() const override;
    
private:
    Point3 m_center;
    float m_radius;
};

} // namespace PathRender

#endif // PATHRENDER_SPHERE_HPP_