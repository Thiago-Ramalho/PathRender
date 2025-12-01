#ifndef F823D023_054D_4B20_9AE2_686DDE1F5C90
#define F823D023_054D_4B20_9AE2_686DDE1F5C90
#ifndef PATHRENDER_LIGHT_HPP_
#define PATHRENDER_LIGHT_HPP_

#include "PathRender/core/point.hpp"
#include <string>
#include <iostream>

namespace PathRender {

class Light {
public:
    
    Light() {}
    Light(const Point3& origin, std::string name, int lp)
        : m_origin(origin), m_name(name), m_lp(lp) {}

    friend std::ostream& operator<<(std::ostream& os, const Light& l) {
        os << "Light(name=" << l.m_name << ", origin=" << l.m_origin << ", lp=" << l.m_lp << ")";
        return os;
    }

private:
    Point3 m_origin;
    std::string m_name;
    int m_lp;
};

} // namespace PathRender

#endif /* PATHRENDER_LIGHT_HPP_ */

#endif /* F823D023_054D_4B20_9AE2_686DDE1F5C90 */
