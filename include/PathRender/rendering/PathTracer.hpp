#ifndef PATHRENDER_PATHTRACER_HPP_
#define PATHRENDER_PATHTRACER_HPP_

#include "PathRender/rendering/IRenderAlgorithm.hpp"
#include <random>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>

namespace PathRender {


class PathTracer : public IRenderAlgorithm {
public:
    PathTracer() = default; 
    void render(std::vector<Color>& buffer, const SceneConfig& config) override;

private:
    Color trace_path(const Ray& ray, int depth, const Scene& scene, std::mt19937& thread_rng);
    Vector3 random_unit_vector_in_hemisphere_of(const Vector3& normal, std::mt19937& thread_rng);
    
    // These helpers are pure math, so they are naturally thread-safe (const input)
    Vector3 refract(const Vector3& uv, const Vector3& n, float etai_over_etat);
    Vector3 mix(const Vector3& a, const Vector3& b, double c);
    float reflectance(float cosine, float ref_idx);
    
    const int max_depth = 5;
};

} // namespace PathRender

#endif // PATHRENDER_PATHTRACER_HPP_