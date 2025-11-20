#ifndef PATHRENDER_VECTOR_HPP_
#define PATHRENDER_VECTOR_HPP_

#include <cmath>

namespace PathRender {

/**
 * @class Vector3
 * @brief Representa um vetor 3D no espaço
 */
class Vector3 {
public:
    float x, y, z;
    
    // Construtores
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    // Operadores aritméticos
    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    
    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    
    Vector3 operator/(float scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
    
    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }
    
    // Operadores de atribuição
    Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    
    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    Vector3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    
    // Produto escalar (dot product)
    float dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    
    // Produto vetorial (cross product)
    Vector3 cross(const Vector3& v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }
    
    // Comprimento
    float length() const {
        return std::sqrt(length_squared());
    }
    
    float length_squared() const {
        return x * x + y * y + z * z;
    }
    
    // Normalização
    Vector3 normalized() const {
        float len = length();
        if (len > 0) {
            return *this / len;
        }
        return *this;
    }
    
    void normalize() {
        float len = length();
        if (len > 0) {
            *this /= len;
        }
    }
};

// Operador para multiplicação escalar à esquerda
inline Vector3 operator*(float scalar, const Vector3& v) {
    return v * scalar;
}

} // namespace PathRender

#endif // PATHRENDER_VECTOR_HPP_
