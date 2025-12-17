#ifndef PATHRENDER_MATERIAL_HPP_
#define PATHRENDER_MATERIAL_HPP_

#include "PathRender/core/color.hpp"
namespace PathRender {

class Material {    
public:
    Material() = default;
    bool is_light;
    Color color;
    Color reflectance;
};

} // namespace PathRender

#endif // PATHRENDER_MATERIAL_HPP_
