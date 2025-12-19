#ifndef PATHRENDER_ANISOTROPICMATTEBRDF_HPP_
#define PATHRENDER_ANISOTROPICMATTEBRDF_HPP_

#include "PathRender/core/BRDF.hpp"
#include "PathRender/utils/math_utils.hpp"

namespace PathRender {

class AnisotropicMatteBRDF : public BRDF {
public:
    // nu: roughness in Tangent direction (0 = sharp, 1 = rough)
    // nv: roughness in Bitangent direction
    AnisotropicMatteBRDF(const Color& col, float nu_val, float nv_val)
        : BRDF(col, 0.0f, 1.0f, 0.0f, 0.0f), nu(nu_val), nv(nv_val) {}

    bool scatter(const Ray& r_in, const HitRecord& hit, ScatterRecord& srec, std::mt19937& rng) const override;
    static Vector3 reflect(const Vector3& v, const Vector3& n);

private:
    float nu, nv; // The two roughness values
};
 
} // namespace PathRender

#endif // PATHRENDER_ANISOTROPICMATTEBRDF_HPP_