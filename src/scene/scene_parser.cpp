#include "PathRender/scene/scene_parser.hpp"
#include "PathRender/objects/plane.hpp"
#include "PathRender/objects/sphere.hpp"
#include <stdexcept>
#include <yaml-cpp/yaml.h>

namespace PathRender {

SceneConfig::SceneConfig(const Scene& scene, const Camera& camera, const OutputParameters& output_params, const Color& background_color) 
    : scene(scene), camera(camera), output_params(output_params), background_color(background_color) {}

SceneConfig SceneParser::parse(const std::string& filename) {
    try {
        YAML::Node root = YAML::LoadFile(filename);
        
        YAML::Node camera_node = root["camera"];
        YAML::Node objects_node = root["objects"];
        YAML::Node background_node = root["background"];
        YAML::Node output_node = root["output"];

        if (!camera_node.IsDefined()) {
            throw std::runtime_error("Configuração da câmera não encontrada no arquivo YAML.");
        }

        if (!objects_node.IsDefined()) {
            throw std::runtime_error("Configuração dos objetos não encontrada no arquivo YAML.");
        }

        if (!output_node.IsDefined()) {
            throw std::runtime_error("Configuração de saída não encontrada no arquivo YAML.");
        }

        if (!background_node.IsDefined()) {
            throw std::runtime_error("Configuração do fundo não encontrada no arquivo YAML.");
        }

        // Output parameters
        OutputParameters output_params;
        output_params.width = output_node["width"].as<int>();
        output_params.height = output_node["height"].as<int>();
        output_params.output_filename = output_node["filename"].as<std::string>();

        // Camera parameters
        double position_x = camera_node["position"][0].as<double>();
        double position_y = camera_node["position"][1].as<double>();
        double position_z = camera_node["position"][2].as<double>();

        double lookAt_x = camera_node["look_at"][0].as<double>();
        double lookAt_y = camera_node["look_at"][1].as<double>();
        double lookAt_z = camera_node["look_at"][2].as<double>();
        
        double up_x = camera_node["up"][0].as<double>();
        double up_y = camera_node["up"][1].as<double>();
        double up_z = camera_node["up"][2].as<double>();

        double fov = camera_node["fov"].as<float>();

        Camera camera(
            Point3(position_x, position_y, position_z),
            Point3(lookAt_x, lookAt_y, lookAt_z),
            Vector3(up_x, up_y, up_z),
            fov,
            static_cast<float>(output_params.width) / static_cast<float>(output_params.height)
        );

        // Background parameters
        double background_r = background_node["color"][0].as<double>();
        double background_g = background_node["color"][1].as<double>();
        double background_b = background_node["color"][2].as<double>();
        Color background_color(background_r, background_g, background_b);

        // Scene objects
        Scene scene;
        for (const auto& obj : objects_node) {
            std::string type = obj["type"].as<std::string>();

            if (type == "sphere") {
                // Read sphere parameters
                double center_x = obj["center"][0].as<double>();
                double center_y = obj["center"][1].as<double>();
                double center_z = obj["center"][2].as<double>();
                Point3 center(center_x, center_y, center_z);

                double radius = obj["radius"].as<double>();

                double color_r = obj["color"][0].as<double>();
                double color_g = obj["color"][1].as<double>();
                double color_b = obj["color"][2].as<double>();
                Color color(color_r, color_g, color_b);

                auto sphere = std::make_shared<Sphere>(center, radius, color);
                scene.add_object(sphere);
            } else if (type == "plane") {
                // Read plane parameters
                double point_x = obj["point"][0].as<double>();
                double point_y = obj["point"][1].as<double>();
                double point_z = obj["point"][2].as<double>();
                Point3 point(point_x, point_y, point_z);

                double normal_x = obj["normal"][0].as<double>();
                double normal_y = obj["normal"][1].as<double>();
                double normal_z = obj["normal"][2].as<double>();
                Vector3 normal(normal_x, normal_y, normal_z);

                double color_r = obj["color"][0].as<double>();
                double color_g = obj["color"][1].as<double>();
                double color_b = obj["color"][2].as<double>();
                Color color(color_r, color_g, color_b);

                auto plane = std::make_shared<Plane>(point, normal, color);
                scene.add_object(plane);
            } else {
                throw std::runtime_error("Tipo de objeto desconhecido: " + type);
            }
        }

        SceneConfig config(scene, camera, output_params, background_color);
        
        return config;
    } catch (const YAML::ParserException &e) {
        throw std::runtime_error("Erro ao analisar o arquivo YAML: " + std::string(e.what()));
    }
}

} // namespace PathRender
