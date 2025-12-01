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
    Point3();
    Point3(float x, float y, float z);
    Point3(const Vector3& v);
    
    // Ponto + Vetor = Ponto
    Point3 operator+(const Vector3& v) const;
    
    // Ponto - Vetor = Ponto
    Point3 operator-(const Vector3& v) const;
    
    // Ponto - Ponto = Vetor
    Vector3 operator-(const Point3& p) const;
    
    // Operadores de atribuição
    Point3& operator+=(const Vector3& v);
    
    Point3& operator-=(const Vector3& v);

    std::string to_string() const;
};

} // namespace PathRender

#endif // PATHRENDER_POINT_HPP_
