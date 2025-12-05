#include "PathRender/rendering/SingleColor.hpp"

namespace PathRender {

void SingleColor::render(std::vector<Color>& buffer) {
    std::fill(buffer.begin(), buffer.end(), Color(1.0f, 0.0f, 0.0f)); // Red
}

} // namespace PathRender
