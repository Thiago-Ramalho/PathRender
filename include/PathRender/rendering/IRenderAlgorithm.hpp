#ifndef PATHRENDER_IRENDERALGORITHM_HPP_
#define PATHRENDER_IRENDERALGORITHM_HPP_

#include "PathRender/core/color.hpp"
#include <vector>

namespace PathRender {

class IRenderAlgorithm {
public:
    virtual ~IRenderAlgorithm() = default;

    virtual void render(std::vector<Color>& buffer) = 0;
};

} // namespace PathRender

#endif // PATHRENDER_IRENDERALGORITHM_HPP_