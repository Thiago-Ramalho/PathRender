#include "PathRender/core/DieletricBRDF.hpp"

namespace PathRender {

Vector3 DielectricBRDF::random_unit_vector(std::mt19937& rng) const {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float z = dist(rng) * 2.0f - 1.0f;
    float a = dist(rng) * M_PI * 2.0f;
    float r = sqrt(1.0f - z * z);
    float x = r * cos(a);
    float y = r * sin(a);
    return Vector3(x, y, z);
}

// Static helper implementation (Math)
Vector3 DielectricBRDF::reflect(const Vector3& v, const Vector3& n) {
    return v - n * 2.0f * v.dot(n);
}

Vector3 DielectricBRDF::refract(const Vector3& uv, const Vector3& n, float etai_over_etat) {
    float cos_theta = fmin((-uv).dot(n), 1.0f);
    Vector3 r_out_perp = (uv + n * cos_theta) * etai_over_etat;
    float r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.length_squared()));
    return r_out_perp + n * r_out_parallel;
}

float DielectricBRDF::reflectance(float cosine, float ref_idx) {
    // Schlick's approximation for reflectance
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

// The main scatter function
bool DielectricBRDF::scatter(const Ray& r_in, const HitRecord& hit, ScatterRecord& srec, std::mt19937& rng) const {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    double total = kd + kt;
    double rayProbability = dist(rng) * total; 
    if (rayProbability < kd) {
        Vector3 direction = (hit.normal + random_unit_vector(rng)).normalized();
        
        // Catch degenerate rays
        if (direction.length() < 0.001f) {
            direction = hit.normal;
        } 

        Point3 hit_point = r_in.origin + r_in.direction * hit.t;
        srec.out_ray = Ray(hit_point + hit.normal * 0.01, direction);
        srec.attenuation = color;
        return true;
    } else {
        // 1. attenuation is always 1 (glass doesn't absorb light, usually)
        srec.attenuation = Color(1.0f, 1.0f, 1.0f); 
        
        // 2. Determine refraction ratio
        // If front_face is true, we are going Air -> Glass (1.0 / ir)
        // If front_face is false, we are going Glass -> Air (ir / 1.0)
        float refraction_ratio = hit.front_face ? (1.0f / ir) : ir;

        Vector3 unit_direction = r_in.direction.normalized();
        
        // 3. Check for Total Internal Reflection
        // Sometimes light cannot escape the glass (critical angle)
        double cos_theta = fmin((-unit_direction).dot(hit.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vector3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > dist(rng)) {
            // Must Reflect
            direction = reflect(unit_direction, hit.normal);
        } else {
            // Can Refract
            direction = refract(unit_direction, hit.normal, refraction_ratio);
        }

        srec.out_ray = Ray(hit.point, direction);
        return true;
    }
}

} // namespace PathRender
