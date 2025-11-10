#include "PathRender/scene/camera.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace PathRender {

Camera::Camera(const Point3& position, const Point3& look_at, const Vector3& up,
               float vfov, float aspect_ratio) {
    m_origin = position;

    m_w = (position - look_at).normalized();
    m_u = up.cross(m_w).normalized();
    m_v = m_w.cross(m_u);

    float theta = vfov * static_cast<float>(M_PI) / 180.0f;
    float h = std::tan(theta / 2.0f);
    float viewport_height = 2.0f * h;
    float viewport_width = aspect_ratio * viewport_height;

    m_horizontal = m_u * viewport_width;
    m_vertical = m_v * viewport_height;
    m_lower_left = m_origin - m_horizontal * 0.5f - m_vertical * 0.5f - m_w;
}

Ray Camera::get_ray(float u, float v) const {
    Vector3 dir = (m_lower_left + m_horizontal * u + m_vertical * v) - m_origin;
    return Ray(m_origin, dir.normalized());
}

} // namespace PathRender
