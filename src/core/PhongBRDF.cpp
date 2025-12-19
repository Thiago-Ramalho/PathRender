#include "PathRender/core/PhongBRDF.hpp"

namespace PathRender {

double PhongBRDF::random(std::mt19937& rng) const {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(rng);
};

Vector3 PhongBRDF::reflect(const Vector3& v, const Vector3& n) const {
    return v - n * 2.0f * v.dot(n);
}

Vector3 PhongBRDF::random_unit_vector(std::mt19937& rng) const {
    float z = random(rng) * 2.0f - 1.0f;
    float a = random(rng) * M_PI * 2.0f;
    float r = sqrt(1.0f - z * z);
    float x = r * cos(a);
    float y = r * sin(a);
    return Vector3(x, y, z);
}

bool PhongBRDF::scatter(const Ray& r_in, const HitRecord& hit, ScatterRecord& srec, std::mt19937& rng) const {
    Point3 hit_point = r_in.origin + r_in.direction * hit.t;
    Vector3 normal = hit.normal;

    double total = kd + ks;
    double rayProbability = random(rng) * total; 
    if (rayProbability < kd) {
        Vector3 direction = (normal + random_unit_vector(rng)).normalized();
        
        // Catch degenerate rays
        if (direction.length() < 0.001f) {
            direction = normal;
        } 

        srec.out_ray = Ray(hit_point + normal * 0.01, direction);
        srec.attenuation = color;
        return true;
    } else {
        Vector3 reflected = reflect(r_in.direction.normalized(), normal);
        
        // Convert 'Shininess' (n) to 'Roughness' (fuzz)
        // Heuristic: n=5 -> fuzz=0.4 (Blurry), n=100 -> fuzz=0.02 (Sharp)
        float fuzz = (n > 0.0f) ? (2.0f / n) : 1.0f; 

        // Clamp fuzz to reasonable limits
        if (fuzz > 1.0f) {
            fuzz = 1.0f;
        }

        // Apply fuzz: Add a random sphere vector to the reflection
        Vector3 direction = (reflected + random_unit_vector(rng) * fuzz).normalized();
        
        // Catch bad scatters (ray going into the surface)
        if (direction.dot(normal) <= 0) {
            return false;
        } 

        srec.out_ray = Ray(hit_point + normal * 0.01, direction);
        srec.attenuation = color;
        return true;
    }
}

} // namespace PathRender
