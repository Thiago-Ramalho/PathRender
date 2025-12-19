#ifndef PATHRENDER_PATHTRACER_HPP_
#define PATHRENDER_PATHTRACER_HPP_

#include "PathRender/rendering/IRenderAlgorithm.hpp"
#include <random>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

namespace PathRender {

struct LightPoint {
    Point3 position;
    Color color;
    float intensity;
};

class PathTracer : public IRenderAlgorithm {
public:
    PathTracer() = default; 
    void render(std::vector<Color>& buffer, const SceneConfig& config) override;
    
    // Direct lighting configuration
    void set_direct_lighting_enabled(bool enabled) { m_direct_lighting_enabled = enabled; }
    bool is_direct_lighting_enabled() const { return m_direct_lighting_enabled; }

private:
    Color trace_path(const Ray& ray, int depth, const Scene& scene, std::mt19937& thread_rng);
    Vector3 random_unit_vector_in_hemisphere_of(const Vector3& normal, std::mt19937& thread_rng);
    
    // Direct lighting methods
    void extract_light_points(const Scene& scene);
    Color calculate_direct_lighting(const Point3& hit_point, const Vector3& normal, 
                                   const Material& material, const Scene& scene, 
                                   std::mt19937& thread_rng);
    bool is_in_shadow(const Point3& point, const Point3& light_pos, const Scene& scene);
    
    // These helpers are pure math, so they are naturally thread-safe (const input)
    Vector3 refract(const Vector3& uv, const Vector3& n, float etai_over_etat);
    Vector3 mix(const Vector3& a, const Vector3& b, double c);
    float reflectance(float cosine, float ref_idx);
    
    const int max_depth = 5;
    
    // Configuration flags
    bool m_direct_lighting_enabled = true;  // Default: enabled
    
    // Light storage
    std::vector<LightPoint> m_light_points;
};

} // namespace PathRender

#endif // PATHRENDER_PATHTRACER_HPP_