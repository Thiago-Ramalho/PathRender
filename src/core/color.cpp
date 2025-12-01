#include "PathRender/core/color.hpp"
#include <algorithm>

namespace PathRender {

Color::Color() : r(0.0), g(0.0), b(0.0) {}

Color::Color(double red, double green, double blue)
    : r(red), g(green), b(blue) {}

Color::Color(int red, int green, int blue)
    : r(red / 255.0), g(green / 255.0), b(blue / 255.0) {}

Color::Color(const Color& other)
    : r(other.r), g(other.g), b(other.b) {}

Color Color::operator*(Color other) const {
    return Color(r * other.r, g * other.g, b * other.b);
}

Color Color::operator*(double scalar) const {
    return Color(r * scalar, g * scalar, b * scalar);
}

Color Color::operator+(Color other) const {
    return Color(r + other.r, g + other.g, b + other.b);
}

Color Color::operator+=(Color other) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

Color& Color::clamp() {
    r = std::max(0.0, std::min(1.0, r));
    g = std::max(0.0, std::min(1.0, g));
    b = std::max(0.0, std::min(1.0, b));
    return *this;
}

std::string Color::to_string() const {
    return "Color(r=" + std::to_string(r) + ", g=" + std::to_string(g) + ", b=" + std::to_string(b) + ")";
}

} // namespace PathRender
