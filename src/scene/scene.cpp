#include "PathRender/scene/scene.hpp"

namespace PathRender {

void Scene::add_object(std::shared_ptr<Object> object) {
    m_objects.push_back(std::move(object));
}

void Scene::add_light(const Light& light) {
    m_lights.push_back(light);
}

const Light& Scene::get_light(size_t index) const {
    return m_lights.at(index);
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

    // // direct lighting (shadows)
    // if(hit_anything) {
    //     for(const auto& object : m_objects) {  // only the lights
    //         if(object->get_material().is_light) {
    //             auto&& light = object;

    //             auto&& collision_point = hit.point + hit.normal * 0.001f; // sanity offset
                
    //             Point3 light_point = light->get_position();

    //             Ray shadow_ray = { collision_point, (light_point - collision_point).normalized() };
                
    //             // find if any object ( that is not this light? ) is between the collision point and the light
    //             HitRecord shadow_hit;
    //             float closest_shadow_object = (light_point - collision_point).length();
    //             for (const auto& obj : m_objects) {
    //                 if (obj->intersect(shadow_ray, t_min, closest_shadow_object, shadow_hit)) {
    //                     if(obj != light) {
    //                         hit.color = Color(.0, .0, .0); // in shadow
    //                         break;
    //                     } 
    //                 }
    //             }
    //         }
    //     }
    // }

    return hit_anything;
}

void Scene::clear() {
    m_objects.clear();
}

size_t Scene::object_count() const {
    return m_objects.size();
}

const std::vector<std::shared_ptr<Object>>& Scene::get_objects() const {
    return m_objects;
}std::string Scene::to_string() const {
    std::string result = "Scene with " + std::to_string(m_objects.size()) + " objects:\n";
    for (const auto& obj : m_objects) {
        result += "  - " + obj->to_string() + "\n";
    }
    return result;
}

} // namespace PathRender
