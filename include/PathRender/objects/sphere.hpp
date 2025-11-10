#ifndef D76A4448_9106_4228_B19D_72F5D7000AAB
#define D76A4448_9106_4228_B19D_72F5D7000AAB
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
    Sphere(const Point3& center, float radius, const Color& color);
    
    /**
     * @brief Testa interseção entre um raio e a esfera
     * 
     * Usa a fórmula quadrática para resolver:
     * (P - C) · (P - C) = r²
     * onde P = origem + t * direção
     */
    bool intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const override;
    
    Color get_color() const override { return m_color; }
    
    Point3 center() const { return m_center; }
    float radius() const { return m_radius; }
    
private:
    Point3 m_center;
    float m_radius;
    Color m_color;
};

} // namespace PathRender

#endif // PATHRENDER_SPHERE_HPP_


#endif /* D76A4448_9106_4228_B19D_72F5D7000AAB */
