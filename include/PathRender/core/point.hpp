#ifndef PATHRENDER_POINT_HPP_
#define PATHRENDER_POINT_HPP_

#include "PathRender/core/vector.hpp"
#include <iostream>

namespace PathRender {

// Forward declaration
class Vector3;

/**
 * @class Point3
 * @brief Representa um ponto no espaço 3D
 */
class Point3 {
public:
    float x, y, z;
    
    // Construtores
    Point3() : x(0), y(0), z(0) {}
    Point3(float x, float y, float z) : x(x), y(y), z(z) {}
    Point3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
    
    // Ponto + Vetor = Ponto
    Point3 operator+(const Vector3& v) const {
        return Point3(x + v.x, y + v.y, z + v.z);
    }
    
    // Ponto - Vetor = Ponto
    Point3 operator-(const Vector3& v) const {
        return Point3(x - v.x, y - v.y, z - v.z);
    }
    
    // Ponto - Ponto = Vetor
    Vector3 operator-(const Point3& p) const {
        return Vector3(x - p.x, y - p.y, z - p.z);
    }
    
    // Operadores de atribuição
    Point3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    
    Point3& operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Point3& p) {
        os << "{" << p.x << ", " << p.y << ", " << p.z << ")";
        return os;
    }
};

} // namespace PathRender

#endif // PATHRENDER_POINT_HPP_
