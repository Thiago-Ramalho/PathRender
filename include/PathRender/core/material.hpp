#ifndef PATHRENDER_MATERIAL_HPP_
#define PATHRENDER_MATERIAL_HPP_

#include "PathRender/core/color.hpp"
namespace PathRender {

class Material {    
public:
    Material() = default;
    Material(bool light, const Color& col, float ambient, float diffuse, float specular, float transmissive, float shininess)
        : is_light(light), color(col), ka(ambient), kd(diffuse), ks(specular), kt(transmissive), n(shininess) {}

    bool is_light;
    Color color;
    float ka, kd, ks, kt;
    float n;    
};
 
} // namespace PathRender

#endif // PATHRENDER_MATERIAL_HPP_
