#include "PathRender/scene/scene_parser.hpp"
#include "PathRender/objects/plane.hpp"
#include "PathRender/objects/sphere.hpp"
#include <stdexcept>
#include <yaml-cpp/yaml.h>

namespace PathRender {

SceneConfig::SceneConfig(const Scene& scene, const Camera& camera, const OutputParameters& output_params, const Color& background_color) 
    : scene(scene), camera(camera), output_params(output_params), background_color(background_color) {}


std::string SceneConfig::to_string() const {
    return "SceneConfig:\n" +
           scene.to_string() + "\n" +
           camera.to_string() + "\n" +
           output_params.to_string() + "\n" +
           "Background Color: " + background_color.to_string();
};

std::string OutputParameters::to_string() const {
    return "OutputParameters(width=" + std::to_string(width) + 
           ", height=" + std::to_string(height) + 
           ", output_filename=" + output_filename + ")\n";
}

} // namespace PathRender
