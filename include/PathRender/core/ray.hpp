#ifndef PATHRENDER_RAY_HPP_
#define PATHRENDER_RAY_HPP_

#include "PathRender/core/point.hpp"
#include "PathRender/core/vector.hpp"

namespace PathRender {

/**
 * @class Ray
 * @brief Representa um raio (semi-reta) no espaço 3D
 * 
 * Um raio é definido por: P(t) = origem + t * direção
 * onde t >= 0
 */
class Ray {
public:
    Point3 origin;
    Vector3 direction;
    
    // Construtores
    Ray() = default;
    Ray(const Point3& origin, const Vector3& direction);
    
    /**
     * @brief Retorna um ponto ao longo do raio
     * @param t Parâmetro do raio (distância)
     * @return Ponto P(t) = origem + t * direção
     */
    Point3 at(float t) const;

    std::string to_string() const;
};

} // namespace PathRender

#endif // PATHRENDER_RAY_HPP_
