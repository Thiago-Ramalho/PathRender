#ifndef PATHRENDER_LIGHT_HPP_
#define PATHRENDER_LIGHT_HPP_

#include "PathRender/core/point.hpp"
#include <string>
#include <iostream>

namespace PathRender {

class Light {
public:
    int m_lp;
    Point3 m_origin;
    std::string m_name;

    Light();

    Light(const Point3& origin, std::string name, int lp);

    std::string to_string() const;
};

} // namespace PathRender

#endif /* PATHRENDER_LIGHT_HPP_ */