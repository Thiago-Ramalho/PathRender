#include "PathRender/scene/yaml_parser.hpp"

namespace PathRender {

SceneConfig YAMLParser::parse(const std::string& filename) {
    try {
        YAML::Node root = YAML::LoadFile(filename);

        OutputParameters output_params = parse_output(root["output"]);
        Camera camera = parse_camera(root["camera"], output_params);
        Scene scene = parse_objects(root["objects"]);
        Color background_color = parse_background(root["background"]);        

        SceneConfig config(scene, camera, output_params, background_color);
        
        return config;
    } catch (const YAML::ParserException &e) {
        throw std::runtime_error("Erro ao analisar o arquivo YAML: " + std::string(e.what()));
    }
}

OutputParameters YAMLParser::parse_output(const YAML::Node& output_node) {
    if (!output_node.IsDefined()) {
        throw std::runtime_error("Configuração de saída não encontrada no arquivo YAML.");
    }

    // Output parameters
    OutputParameters output_params;
    output_params.width = output_node["width"].as<int>();
    output_params.height = output_node["height"].as<int>();
    output_params.output_filename = output_node["filename"].as<std::string>();

    return output_params;
}

Camera YAMLParser::parse_camera(const YAML::Node& camera_node, const OutputParameters& output_params) {
    if (!camera_node.IsDefined()) {
        throw std::runtime_error("Configuração da câmera não encontrada no arquivo YAML.");
    }

    double fov = camera_node["fov"].as<float>();
    Camera camera(
        parse_point3(camera_node["position"]),
        parse_point3(camera_node["look_at"]),
        parse_vector3(camera_node["up"]),
        fov,
        static_cast<float>(output_params.width) / static_cast<float>(output_params.height)
    );

    return camera;
}

std::shared_ptr<Sphere> YAMLParser::parse_sphere(const YAML::Node& node) {
    Point3 center = parse_point3(node["center"]);
    double radius = node["radius"].as<double>();
    Point3 color_point = parse_point3(node["color"]);
    Color color(color_point.x, color_point.y, color_point.z);

    return std::make_shared<Sphere>(center, radius, color);
}

std::shared_ptr<Plane> YAMLParser::parse_plane(const YAML::Node& node) {
    Point3 point = parse_point3(node["point"]);
    Vector3 normal = parse_vector3(node["normal"]);
    Point3 color_point = parse_point3(node["color"]);
    Color color(color_point.x, color_point.y, color_point.z);

    return std::make_shared<Plane>(point, normal, color);
}

Scene YAMLParser::parse_objects(const YAML::Node& objects_node) {
    if (!objects_node.IsDefined()) {
        throw std::runtime_error("Configuração dos objetos não encontrada no arquivo YAML.");
    }

    // Scene objects
    Scene scene;
    for (const auto& obj : objects_node) {
        std::string type = obj["type"].as<std::string>();

        if (type == "sphere") {
            auto sphere = parse_sphere(obj);
            scene.add_object(sphere);
        } else if (type == "plane") {
            auto plane = parse_plane(obj);
            scene.add_object(plane);
        } else {
            throw std::runtime_error("Tipo de objeto desconhecido: " + type);
        }
    }

    return scene;
}

Color YAMLParser::parse_background(const YAML::Node& background_node) {
    if (!background_node.IsDefined()) {
        throw std::runtime_error("Configuração do fundo não encontrada no arquivo YAML.");
    }

    Point3 color_point = parse_point3(background_node["color"]);
    Color background_color(color_point.x, color_point.y, color_point.z);

    return background_color;
}

} // namespace PathRender