#ifndef PATHRENDER_BRDF_HPP_
#define PATHRENDER_BRDF_HPP_

#include "PathRender/core/HitRecord.hpp"
#include "PathRender/core/ScatterRecord.hpp"
#include "PathRender/core/color.hpp"
#include "PathRender/core/ray.hpp"
#include <random>

namespace PathRender {

class BRDF {    
public:
    BRDF(const Color& col, float ambient, float diffuse, float specular, float transmissive, float shininess)
        : color(col), ka(ambient), kd(diffuse), ks(specular), kt(transmissive), n(shininess) {}
    virtual ~BRDF() = default;

    Color color;
    float ka, kd, ks, kt, n;

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec, std::mt19937& rng) const = 0;
};
 
} // namespace PathRender

#endif // PATHRENDER_BRDF_HPP_