#ifndef PATHRENDER_PATHTRACER_HPP_
#define PATHRENDER_PATHTRACER_HPP_

#include "PathRender/rendering/IRenderAlgorithm.hpp"

namespace PathRender {

class PathTracer : public IRenderAlgorithm {
public:
    void render(std::vector<Color>& buffer, const SceneConfig& config) override;
private:
    Color trace_path(const Ray& ray, int depth, const Scene& scene);
    Vector3 random_unit_vector_in_hemisphere_of(const Vector3& normal);
    Vector3 reflect(const Vector3& v, const Vector3& n);
    Vector3 refract(const Vector3& uv, const Vector3& n, float etai_over_etat);
    Vector3 mix(const Vector3& a, const Vector3& b, double c);
    float reflectance(float cosine, float ref_idx);
    Vector3 RandomUnitVector();
    const int max_depth = 5;
    int64_t seed_state;

    double random() {
        seed_state = (1103515245 * seed_state + 12345) % 2147483647;
        return static_cast<float>(seed_state) / static_cast<float>(2147483647);
    };

};

} // namespace PathRender

#endif // PATHRENDER_PATHTRACER_HPP_