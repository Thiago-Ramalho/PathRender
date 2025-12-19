#ifndef PATHRENDER_OBJECTS_HPP_
#define PATHRENDER_OBJECTS_HPP_

#include "PathRender/core/HitRecord.hpp"
#include "PathRender/core/ray.hpp"
#include "PathRender/core/point.hpp"
#include "PathRender/core/material.hpp"
#include "PathRender/core/vector.hpp"
#include "PathRender/core/color.hpp"
#include <memory>

namespace PathRender {

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

protected:
    Material m_material;
};

} // namespace PathRender

#endif // PATHRENDER_OBJECTS_HPP_