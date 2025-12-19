#include "PathRender/scene/yaml_parser.hpp"

namespace PathRender {

SceneConfig YAMLParser::parse(const std::string& filename) {
    try {
        std::cout << "Carregando arquivo YAML: " << filename << std::endl;
        YAML::Node root = YAML::LoadFile(filename);
        
        if (!root) {
            throw std::runtime_error("Arquivo YAML vazio ou inválido: " + filename);
        }

        std::cout << "Parseando configurações de saída..." << std::endl;
        OutputParameters output_params = parse_output(root["output"]);
        
        std::cout << "Parseando câmera..." << std::endl;
        Camera camera = parse_camera(root["camera"], output_params);
        
        std::cout << "Parseando objetos..." << std::endl;
        Scene scene = parse_objects(root["objects"]);
        
        std::cout << "Parseando cor de fundo..." << std::endl;
        Color background_color = parse_background(root["background"]);        

        SceneConfig config(scene, camera, output_params, background_color);
        std::cout << "Arquivo YAML carregado com sucesso!" << std::endl;
        
        return config;
    } catch (const YAML::ParserException &e) {
        throw std::runtime_error("Erro ao analisar o arquivo YAML '" + filename + "': " + std::string(e.what()));
    } catch (const YAML::BadFile &e) {
        throw std::runtime_error("Não foi possível abrir o arquivo YAML: " + filename);
    } catch (const std::exception &e) {
        throw std::runtime_error("Erro inesperado ao carregar YAML '" + filename + "': " + std::string(e.what()));
    }
}

OutputParameters YAMLParser::parse_output(const YAML::Node& output_node) {
    if (!output_node.IsDefined()) {
        throw std::runtime_error("Seção 'output' não encontrada no arquivo YAML.");
    }

    try {
        OutputParameters output_params;
        
        if (!output_node["width"]) {
            throw std::runtime_error("Campo 'width' não encontrado na seção 'output'.");
        }
        output_params.width = output_node["width"].as<int>();
        
        if (!output_node["height"]) {
            throw std::runtime_error("Campo 'height' não encontrado na seção 'output'.");
        }
        output_params.height = output_node["height"].as<int>();
        
        if (!output_node["filename"]) {
            throw std::runtime_error("Campo 'filename' não encontrado na seção 'output'.");
        }
        output_params.output_filename = output_node["filename"].as<std::string>();

        std::cout << "  Resolução: " << output_params.width << "x" << output_params.height << std::endl;
        return output_params;
    } catch (const YAML::Exception &e) {
        throw std::runtime_error("Erro ao parsear seção 'output': " + std::string(e.what()));
    }
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

// 1. New Material Parser
Material YAMLParser::parse_material(const YAML::Node& node) {
    // Default Material
    if (!node.IsDefined()) {
        return Material(false, std::make_shared<PhongBRDF>(Color(0.5, 0.5, 0.5)));
    }

    std::string type = node["type"] ? node["type"].as<std::string>() : "phong";
    Point3 color_pt = parse_point3(node["color"]);
    Color color(color_pt.x, color_pt.y, color_pt.z);
    
    bool is_light = node["is_light"] && node["is_light"].as<bool>();

    if (type == "anisotropic") {
        float nu = node["roughness_u"] ? node["roughness_u"].as<float>() : 0.5f;
        float nv = node["roughness_v"] ? node["roughness_v"].as<float>() : 0.5f;
        return Material(is_light, std::make_shared<AnisotropicMatteBRDF>(color, nu, nv));
    }
    
    // Default to Phong
    return Material(is_light, std::make_shared<PhongBRDF>(color));
}

// 2. Updated Object Parsers to use parse_material
std::shared_ptr<Sphere> YAMLParser::parse_sphere(const YAML::Node& node) {
    Point3 center = parse_point3(node["center"]);
    double radius = node["radius"].as<double>();
    Material material = parse_material(node["material"]);
    return std::make_shared<Sphere>(center, radius, material);
}

// std::shared_ptr<Sphere> YAMLParser::parse_sphere(const YAML::Node& node) {
//     Point3 center = parse_point3(node["center"]);
//     double radius = node["radius"].as<double>();
//     Point3 color_point = parse_point3(node["color"]);
//     Color color(color_point.x, color_point.y, color_point.z);
//     Material material = {false, std::make_shared<PhongBRDF>(color)};

//     return std::make_shared<Sphere>(center, radius, material);
// }

std::shared_ptr<Plane> YAMLParser::parse_plane(const YAML::Node& node) {
    Point3 point = parse_point3(node["point"]);
    Vector3 normal = parse_vector3(node["normal"]);
    Material material = parse_material(node["material"]);
    return std::make_shared<Plane>(point, normal, material);
}

// std::shared_ptr<Plane> YAMLParser::parse_plane(const YAML::Node& node) {
//     Point3 point = parse_point3(node["point"]);
//     Vector3 normal = parse_vector3(node["normal"]);
//     Point3 color_point = parse_point3(node["color"]);
//     Color color(color_point.x, color_point.y, color_point.z);
//     Material material = {false, std::make_shared<PhongBRDF>(color)};

//     return std::make_shared<Plane>(point, normal, material);
// }

// 3. New Quad Parser (Converts 4 points -> 2 Triangles)
std::shared_ptr<Mesh> YAMLParser::parse_quad(const YAML::Node& node) {
    if (!node["points"] || node["points"].size() != 4) {
        throw std::runtime_error("Quad object must have exactly 4 points.");
    }

    auto mesh = std::make_shared<Mesh>();

    // 1. Parse and store Vertices
    std::vector<Point3> p;
    for (int i = 0; i < 4; ++i) {
        Point3 pt = parse_point3(node["points"][i]);
        p.push_back(pt);
        mesh->add_vertex(pt); // Add to mesh storage (matches OBJParser logic)
    }

    // 2. Parse Material
    Material material = parse_material(node["material"]);
    mesh->set_material(material); // Set material on the Mesh wrapper

    // 3. Create Triangles
    // Triangle 1: Vertices 0, 1, 2
    Triangle t1(p[0], p[1], p[2], material);
    mesh->add_triangle(t1);

    // Triangle 2: Vertices 0, 2, 3
    Triangle t2(p[0], p[2], p[3], material);
    mesh->add_triangle(t2);

    return mesh;



    // std::vector<std::shared_ptr<Object>> triangles;
    
    // if (!node["points"] || node["points"].size() != 4) {
    //     throw std::runtime_error("Quad object must have exactly 4 points.");
    // }

    // std::vector<Point3> p;
    // for (int i = 0; i < 4; ++i) {
    //     p.push_back(parse_point3(node["points"][i]));
    // }

    // Material material = parse_material(node["material"]);

    // // Create two triangles: (0,1,2) and (0,2,3)
    // // Assuming you have a Triangle class: Triangle(p0, p1, p2, material)
    // triangles.push_back(std::make_shared<Triangle>(p[0], p[1], p[2], material));
    // triangles.push_back(std::make_shared<Triangle>(p[0], p[2], p[3], material));

    // return triangles;
}

// 4. Updated Main Object Loop
Scene YAMLParser::parse_objects(const YAML::Node& objects_node) {
    if (!objects_node.IsDefined() || !objects_node.IsSequence()) {
        throw std::runtime_error("Seção 'objects' inválida.");
    }

    Scene scene;
    int object_count = 0;
    
    for (const auto& obj : objects_node) {
        if (!obj["type"]) continue;
        std::string type = obj["type"].as<std::string>();
        
        std::cout << "  Loading " << type << "..." << std::endl;

        if (type == "sphere") {
            scene.add_object(parse_sphere(obj));
        } else if (type == "plane") {
            scene.add_object(parse_plane(obj));
        } else if (type == "quad") {
            // Updated logic: Receive a single Mesh object instead of a list of triangles
            std::shared_ptr<Mesh> mesh = parse_quad(obj);
            scene.add_object(mesh);

            // // Quads return multiple triangles
            // auto tris = parse_quad(obj);
            // for (auto& tri : tris) {
            //     scene.add_object(tri);
            // }
        }
        object_count++;
    }

    return scene;
}

// Scene YAMLParser::parse_objects(const YAML::Node& objects_node) {
//     if (!objects_node.IsDefined()) {
//         throw std::runtime_error("Seção 'objects' não encontrada no arquivo YAML.");
//     }

//     if (!objects_node.IsSequence()) {
//         throw std::runtime_error("Seção 'objects' deve ser uma lista/array.");
//     }

//     Scene scene;
//     int object_count = 0;
    
//     for (const auto& obj : objects_node) {
//         try {
//             if (!obj["type"]) {
//                 throw std::runtime_error("Campo 'type' não encontrado no objeto " + std::to_string(object_count));
//             }
            
//             std::string type = obj["type"].as<std::string>();
//             std::cout << "  Processando objeto " << object_count << " do tipo: " << type << std::endl;

//             if (type == "sphere") {
//                 auto sphere = parse_sphere(obj);
//                 scene.add_object(sphere);
//             } else if (type == "plane") {
//                 auto plane = parse_plane(obj);
//                 scene.add_object(plane);
//             } else {
//                 throw std::runtime_error("Tipo de objeto desconhecido: " + type);
//             }
            
//             object_count++;
//         } catch (const std::exception &e) {
//             throw std::runtime_error("Erro ao parsear objeto " + std::to_string(object_count) + ": " + std::string(e.what()));
//         }
//     }

//     std::cout << "  Total de objetos carregados: " << object_count << std::endl;
//     return scene;
// }

Color YAMLParser::parse_background(const YAML::Node& background_node) {
    if (!background_node.IsDefined()) {
        throw std::runtime_error("Configuração do fundo não encontrada no arquivo YAML.");
    }

    Point3 color_point = parse_point3(background_node["color"]);
    Color background_color(color_point.x, color_point.y, color_point.z);

    return background_color;
}

Point3 YAMLParser::parse_point3(const YAML::Node& node) {
    double x = node[0].as<double>();
    double y = node[1].as<double>();
    double z = node[2].as<double>();
    return Point3(x, y, z);
}

Vector3 YAMLParser::parse_vector3(const YAML::Node& node) {
    Point3 point3 = parse_point3(node);
    return Vector3(point3.x, point3.y, point3.z);
}

} // namespace PathRender