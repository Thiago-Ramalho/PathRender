#ifndef PATHRENDER_CAMERA_HPP_
#define PATHRENDER_CAMERA_HPP_

#include "PathRender/core/point.hpp"
#include "PathRender/core/vector.hpp"
#include "PathRender/core/ray.hpp"

namespace PathRender {

/**
 * @class Camera
 * @brief Câmera virtual para gerar raios
 */
class Camera {
public:
    /**
     * @brief Constrói uma câmera
     * @param position Posição da câmera no espaço
     * @param look_at Ponto para onde a câmera está olhando
     * @param up Vetor "up" da câmera
     * @param vfov Campo de visão vertical em graus
     * @param aspect_ratio Razão largura/altura
     */
    Camera(const Point3& position, 
           const Point3& look_at, 
           const Vector3& up,
           float vfov, 
           float aspect_ratio);
    
    /**
     * @brief Gera um raio através de um pixel na tela
     * @param u Coordenada horizontal normalizada [0, 1]
     * @param v Coordenada vertical normalizada [0, 1]
     * @return Raio da câmera através do pixel (u, v)
     */
    Ray get_ray(float u, float v) const;
    
    std::string to_string() const {
        return "Camera(origin=" + std::to_string(m_origin.x) + ", " + std::to_string(m_origin.y) + ", " + std::to_string(m_origin.z) + ")";
    }

private:
    Point3 m_origin;          // Posição da câmera
    Point3 m_lower_left;      // Canto inferior esquerdo do plano de visão
    Vector3 m_horizontal;     // Vetor horizontal do plano de visão
    Vector3 m_vertical;       // Vetor vertical do plano de visão
    
    // Base ortonormal da câmera
    Vector3 m_u, m_v, m_w;
};

} // namespace PathRender

#endif // PATHRENDER_CAMERA_HPP_