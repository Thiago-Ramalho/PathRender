#ifndef PATHRENDER_HITRECORD_HPP_
#define PATHRENDER_HITRECORD_HPP_

#include "PathRender/core/point.hpp"
#include "PathRender/core/ray.hpp"
#include <memory>
namespace PathRender {

class Object;
    
/**
 * @struct HitRecord
 * @brief Armazena informações sobre uma interseção entre um raio e um objeto
 */
struct HitRecord {
    float t;              // Parâmetro t do raio onde ocorreu a interseção
    Point3 point;         // Ponto de interseção no espaço 3D
    Vector3 normal;       // Normal da superfície no ponto de interseção
    std::shared_ptr<Object> object; // Ponteiro para o objeto atingido
    bool front_face;      // True se o raio atingiu a face frontal
    
    /**
     * @brief Define a normal para apontar contra o raio
     * @param ray O raio que causou a interseção
     * @param outward_normal A normal que aponta para fora da superfície
     */
    void set_face_normal(const Ray& ray, const Vector3& outward_normal) {
        front_face = ray.direction.dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};
 
} // namespace PathRender

#endif // PATHRENDER_HITRECORD_HPP_