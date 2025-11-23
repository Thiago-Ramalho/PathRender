#include "PathRender/scene/scene_parser.hpp"
#include "PathRender/objects/plane.hpp"
#include "PathRender/objects/sphere.hpp"
#include <stdexcept>
#include <yaml-cpp/yaml.h>

namespace PathRender {

SceneConfig::SceneConfig(const Scene& scene, const Camera& camera, const OutputParameters& output_params, const Color& background_color) 
    : scene(scene), camera(camera), output_params(output_params), background_color(background_color) {}

Point3 SceneParser::parse_point3(const YAML::Node& node) {
    double x = node[0].as<double>();
    double y = node[1].as<double>();
    double z = node[2].as<double>();
    return Point3(x, y, z);
}

Vector3 SceneParser::parse_vector3(const YAML::Node& node) {
    Point3 point3 = parse_point3(node);
    return Vector3(point3.x, point3.y, point3.z);
}

} // namespace PathRender
