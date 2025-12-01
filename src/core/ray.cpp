#include "PathRender/core/ray.hpp"

namespace PathRender {

Ray::Ray(const Point3& origin, const Vector3& direction)
        : origin(origin), direction(direction) {}
    
Point3 Ray::at(float t) const {
    return origin + direction * t;
}

std::string Ray::to_string() const {
    return "Ray(origin=" + origin.to_string() + ", direction=" + direction.to_string() + ")";
}

} // namespace PathRender
