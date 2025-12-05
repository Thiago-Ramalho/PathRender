#ifndef PATHRENDER_OBJECTS_HPP_
#define PATHRENDER_OBJECTS_HPP_

#include "PathRender/core/ray.hpp"
#include "PathRender/core/point.hpp"
#include "PathRender/core/material.hpp"
#include "PathRender/core/vector.hpp"
#include "PathRender/core/color.hpp"
#include <memory>

namespace PathRender {

/**
 * @struct HitRecord
 * @brief Armazena informações sobre uma interseção entre um raio e um objeto
 */
struct HitRecord {
    float t;              // Parâmetro t do raio onde ocorreu a interseção
    Point3 point;         // Ponto de interseção no espaço 3D
    Vector3 normal;       // Normal da superfície no ponto de interseção
    Color color;          // Cor do objeto no ponto de interseção
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

/**
 * @class Object
 * @brief Classe base abstrata para todos os objetos renderizáveis
 */
class Object {
public:
    virtual ~Object() = default;
    
    /**
     * @brief Testa interseção entre um raio e o objeto
     * @param ray O raio a testar
     * @param t_min Distância mínima válida ao longo do raio
     * @param t_max Distância máxima válida ao longo do raio
     * @param hit Estrutura para armazenar informações da interseção
     * @return true se houver interseção, false caso contrário
     */
    virtual bool intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const = 0;
    
    /**
     * @brief Retorna a cor do objeto
     */
    virtual const Color& get_color() const = 0;

    virtual std::string to_string() const = 0;

    void set_material(const Material& material) {
        m_material = material; 
    }

    const Material& get_material() const {
        return m_material; 
    }

    virtual Point3 get_position() const = 0;

private:
    Material m_material;
};

} // namespace PathRender

#endif // PATHRENDER_OBJECTS_HPP_