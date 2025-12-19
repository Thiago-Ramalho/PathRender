#ifndef PATHRENDER_SCATTERRECORD_HPP_
#define PATHRENDER_SCATTERRECORD_HPP_

#include "PathRender/core/HitRecord.hpp"
#include "PathRender/core/color.hpp"
#include "PathRender/core/ray.hpp"
#include <random>

namespace PathRender {

struct ScatterRecord {
    Ray out_ray;        // Raio espalhado
    Color attenuation;   // Atenuação da cor
};
 
} // namespace PathRender

#endif // PATHRENDER_SCATTERRECORD_HPP_