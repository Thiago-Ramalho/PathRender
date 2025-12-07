#ifndef PATHRENDER_RAYCAST_HPP_
#define PATHRENDER_RAYCAST_HPP_

#include "PathRender/rendering/IRenderAlgorithm.hpp"

namespace PathRender {

class RayCast : public IRenderAlgorithm {
public:
    void render(std::vector<Color>& buffer, const SceneConfig& config) override;
};

} // namespace PathRender

#endif // PATHRENDER_RAYCAST_HPP_