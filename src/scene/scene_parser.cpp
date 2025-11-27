#include "PathRender/scene/scene_parser.hpp"
#include "PathRender/objects/plane.hpp"
#include "PathRender/objects/sphere.hpp"
#include <stdexcept>
#include <yaml-cpp/yaml.h>

namespace PathRender {

SceneConfig::SceneConfig(const Scene& scene, const Camera& camera, const OutputParameters& output_params, const Color& background_color) 
    : scene(scene), camera(camera), output_params(output_params), background_color(background_color) {}

} // namespace PathRender
