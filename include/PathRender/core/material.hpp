#ifndef PATHRENDER_MATERIAL_HPP_
#define PATHRENDER_MATERIAL_HPP_

#include "PathRender/core/BRDF.hpp"
#include "PathRender/core/color.hpp"
#include <memory>

namespace PathRender {

class Material {    
public:
    Material() = default;
    Material(bool light, std::shared_ptr<BRDF> brdf_ptr) : is_light(light), brdf(std::move(brdf_ptr)) {}

    bool is_light;
    std::shared_ptr<BRDF> brdf;   
};
 
} // namespace PathRender

#endif // PATHRENDER_MATERIAL_HPP_
