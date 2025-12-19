#ifndef PATHRENDER_MATERIAL_HPP_
#define PATHRENDER_MATERIAL_HPP_

#include "PathRender/core/BRDF.hpp"
#include "PathRender/core/color.hpp"
#include <memory>

namespace PathRender {

class Material {    
public:
    Material() = default;
    Material(bool light, const Color& col, float ambient, float diffuse, float specular, float transmissive, float shininess, std::unique_ptr<BRDF>&& brdf_ptr)
        : is_light(light), brdf(std::move(brdf_ptr)), color(col), ka(ambient), kd(diffuse), ks(specular), kt(transmissive), n(shininess) {}
    Material(bool light, std::unique_ptr<BRDF>&& brdf_ptr) : is_light(light), brdf(std::move(brdf_ptr)) {}

    bool is_light;
    std::unique_ptr<BRDF> brdf;
    Color color;
    float ka, kd, ks, kt;
    float n;    
};
 
} // namespace PathRender

#endif // PATHRENDER_MATERIAL_HPP_
