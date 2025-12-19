#ifndef PATHRENDER_PHONGBRDF_HPP_
#define PATHRENDER_PHONGBRDF_HPP_

#include "PathRender/core/BRDF.hpp"

namespace PathRender {

class PhongBRDF : public BRDF {    
public:
    PhongBRDF(const Color& col) : BRDF(col, 0.3f, 0.7f, 0.0f, 0.0f, 5.0f) {}

    bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec, std::mt19937& rng) const override;
};
 
} // namespace PathRender

#endif // PATHRENDER_PHONGBRDF_HPP_