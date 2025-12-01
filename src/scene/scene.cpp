#include "PathRender/scene/scene.hpp"

namespace PathRender {

void Scene::add_object(std::shared_ptr<Object> object) {
    m_objects.push_back(std::move(object));
}

void Scene::add_light(const Light& light) {
    m_lights.push_back(light);
}

bool Scene::intersect(const Ray& ray, float t_min, float t_max, HitRecord& hit) const {
    HitRecord temp_hit;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (const auto& obj : m_objects) {
        if (obj->intersect(ray, t_min, closest_so_far, temp_hit)) {
            hit_anything = true;
            closest_so_far = temp_hit.t;
            hit = temp_hit;
        }
    }

    return hit_anything;
}

void Scene::clear() {
    m_objects.clear();
}

size_t Scene::object_count() const { 
    return m_objects.size(); 
}

} // namespace PathRender
