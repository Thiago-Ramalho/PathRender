#include "PathRender/core/AnisotropicMatteBRDF.hpp"

namespace PathRender {

// Static helper implementation (Math)
Vector3 AnisotropicMatteBRDF::reflect(const Vector3& v, const Vector3& n) {
    return v - n * 2.0f * v.dot(n);
}

bool AnisotropicMatteBRDF::scatter(const Ray& r_in, const HitRecord& hit, ScatterRecord& srec, std::mt19937& rng) const {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // 1. Calculate Perfect Reflection
    Vector3 reflected = reflect(r_in.direction, hit.normal);

    // 2. Build the Surface Basis (T, B, N)
    Vector3 tangent, bitangent;
    
    Utils::build_orthonormal_basis(hit.normal, tangent, bitangent);

    // 3. Generate Random Point in Unit Sphere
    // We reuse your logic for random unit sphere generation
    auto random_in_unit_sphere = [&]() {
        while(true) {
            float x = dist(rng) * 2.0f - 1.0f;
            float y = dist(rng) * 2.0f - 1.0f;
            float z = dist(rng) * 2.0f - 1.0f;
            if(x*x + y*y + z*z <= 1) return Vector3(x,y,z);
        }
    };
    Vector3 rand_vec = random_in_unit_sphere();

    // 4. Apply Anisotropic Scaling
    // Instead of adding 'rand_vec * fuzz', we scale the components differently.
    // We treat the reflection direction as the "Z-axis" of our perturbation space.
    
    // We need a basis aligned with the REFLECTION vector, not the SURFACE normal
    Vector3 r_u, r_v;
    Utils::build_orthonormal_basis(reflected, r_u, r_v);

    // Scale the perturbation:
    // If nu is high (0.8), we add a lot of noise in the U direction (blurry).
    // If nv is low (0.1), we add very little noise in the V direction (sharp).
    Vector3 perturbation = (r_u * rand_vec.x * nu) + (r_v * rand_vec.y * nv) + (reflected * rand_vec.z * 0.0f);

    Vector3 final_direction = (reflected + perturbation).normalized();

    // 5. Safety check: Ensure we didn't scatter into the surface
    if (final_direction.dot(hit.normal) <= 0) {
        // Simple fix: clamp it or just return the perfect reflection
        final_direction = reflected; 
    }

    srec.out_ray = Ray(hit.point + hit.normal * 0.001f, final_direction);
    srec.attenuation = color;
    return true;
}

} // namespace PathRender
