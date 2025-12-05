#ifndef PATHRENDER_SINGLECOLOR_HPP_
#define PATHRENDER_SINGLECOLOR_HPP_

#include "PathRender/rendering/IRenderAlgorithm.hpp"

namespace PathRender {

class SingleColor : public IRenderAlgorithm {
public:
    void render(std::vector<Color>& buffer) override;
};

} // namespace PathRender

#endif // PATHRENDER_SINGLECOLOR_HPP_