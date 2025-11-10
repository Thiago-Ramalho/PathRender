#ifndef C0594608_D531_4086_80A2_D999DC7D319D
#define C0594608_D531_4086_80A2_D999DC7D319D
#ifndef PATHRENDER_SCENE_HPP_
#define PATHRENDER_SCENE_HPP_

#include "PathRender/objects/objects.hpp"
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
    size_t object_count() const { return m_objects.size(); }
    
private:
    std::vector<std::shared_ptr<Object>> m_objects;
};

} // namespace PathRender

#endif // PATHRENDER_SCENE_HPP_


#endif /* C0594608_D531_4086_80A2_D999DC7D319D */
