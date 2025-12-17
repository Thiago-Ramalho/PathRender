#ifndef PATHRENDER_SCENE_HPP_
#define PATHRENDER_SCENE_HPP_

#include "PathRender/objects/objects.hpp"
#include "PathRender/core/light.hpp"
#include "PathRender/core/ray.hpp"
#include <vector>
#include <memory>

namespace PathRender {

/**
 * @class Scene
 * @brief Container para todos os objetos da cena
 */
class Scene {
public:
    Scene() = default;
    
    /**
     * @brief Adiciona um objeto à cena
     * @param object Ponteiro compartilhado para o objeto
     */
    void add_object(std::shared_ptr<Object> object);

    void add_light(const Light& light);

    const Light& get_light(size_t index) const;
    
    /**
     * @brief Testa interseção do raio com todos os objetos da cena
     * @param ray O raio a testar
     * @param t_min Distância mínima válida
     * @param t_max Distância máxima válida
     * @param hit Estrutura para armazenar a interseção mais próxima
     * @return true se houver interseção, false caso contrário
     */
    bool intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const;
    
    /**
     * @brief Remove todos os objetos da cena
     */
    void clear();
    
    /**
     * @brief Retorna o número de objetos na cena
     */
    size_t object_count() const;

    std::string to_string() const;
    
private:
    std::vector<Light> m_lights;
    std::vector<std::shared_ptr<Object>> m_objects;
};

} // namespace PathRender

#endif // PATHRENDER_SCENE_HPP_