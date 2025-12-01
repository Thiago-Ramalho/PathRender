#include "PathRender/core/light.hpp"

namespace PathRender {

Light::Light() : m_lp(0), m_origin(Point3(0.0, 0.0, 0.0)), m_name("light") {}

Light::Light(const Point3& origin, std::string name, int lp)
    : m_lp(lp), m_origin(origin), m_name(name) {}

std::string Light::to_string() const {
    return "Light(name=" + m_name + ", origin=" + m_origin.to_string() + ", lp=" + std::to_string(m_lp) + ")";
}

} // namespace PathRender
