#include "PathRender/core/point.hpp"

namespace PathRender {

Point3::Point3() : x(0), y(0), z(0) {}

Point3::Point3(float x, float y, float z) : x(x), y(y), z(z) {}

Point3::Point3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

// Ponto + Vetor = Ponto
Point3 Point3::operator+(const Vector3& v) const {
    return Point3(x + v.x, y + v.y, z + v.z);
}

// Ponto - Vetor = Ponto
Point3 Point3::operator-(const Vector3& v) const {
    return Point3(x - v.x, y - v.y, z - v.z);
}

// Ponto - Ponto = Vetor
Vector3 Point3::operator-(const Point3& p) const {
    return Vector3(x - p.x, y - p.y, z - p.z);
}

// Operadores de atribuição
Point3& Point3::operator+=(const Vector3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Point3& Point3::operator-=(const Vector3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

std::string Point3::to_string() const {
    return "Point3(x=" + std::to_string(x) + ", y=" + std::to_string(y) + ", z=" + std::to_string(z) + ")";
}

} // namespace PathRender
