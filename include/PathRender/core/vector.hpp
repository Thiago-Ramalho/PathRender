#ifndef PATHRENDER_VECTOR_HPP_
#define PATHRENDER_VECTOR_HPP_

#include <cmath>
#include <string>

namespace PathRender {

/**
 * @class Vector3
 * @brief Representa um vetor 3D no espaço
 */
class Vector3 {
public:
    float x, y, z;
    
    // Construtores
    Vector3();
    Vector3(float x, float y, float z);
    
    // Operadores aritméticos
    Vector3 operator+(const Vector3& v) const;
    
    Vector3 operator-(const Vector3& v) const;
    
    Vector3 operator*(float scalar) const;
    
    Vector3 operator/(float scalar) const;
    
    Vector3 operator-() const;
    
    // Operadores de atribuição
    Vector3& operator+=(const Vector3& v);
    
    Vector3& operator*=(float scalar);
    
    Vector3& operator/=(float scalar);
    
    // Produto escalar (dot product)
    float dot(const Vector3& v) const;
    
    // Produto vetorial (cross product)
    Vector3 cross(const Vector3& v) const;
    
    // Comprimento
    float length() const;
    
    float length_squared() const;
    
    // Normalização
    Vector3 normalized() const;
    
    void normalize();

    std::string to_string() const;
};

} // namespace PathRender

#endif // PATHRENDER_VECTOR_HPP_
