#ifndef PATHRENDER_DIELETRICBRDF_HPP_
#define PATHRENDER_DIELETRICBRDF_HPP_

#include "PathRender/core/BRDF.hpp"

namespace PathRender {

class DielectricBRDF : public BRDF {
public:
    // ref_idx is the Index of Refraction (1.5 for glass, 2.4 for diamond)
    DielectricBRDF(const Color& col, float ref_idx) 
        : BRDF(col, 0.3f, 0.0f, 0.7f, 0.0f), ir(ref_idx) {}

    bool scatter(const Ray& r_in, const HitRecord& hit, ScatterRecord& srec, std::mt19937& rng) const override;

private:
    float ir; // Index of Refraction

    // Helper functions specific to physics of refraction
    static float reflectance(float cosine, float ref_idx);
    static Vector3 refract(const Vector3& uv, const Vector3& n, float etai_over_etat);
    static Vector3 reflect(const Vector3& v, const Vector3& n);
    Vector3 random_unit_vector(std::mt19937& rng) const;
};
 
} // namespace PathRender

#endif // PATHRENDER_DIELETRICBRDF_HPP_