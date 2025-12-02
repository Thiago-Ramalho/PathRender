#include "PathRender/scene/obj_parser.hpp"
#include "PathRender/core/light.hpp"
#include <fstream>
#include <iostream>

namespace PathRender {

SceneConfig OBJParser::parse(const std::string& filename) {
    return parse_scene(filename);
}

Vector3 OBJParser::parse_vector3(const std::string& line) {
    std::stringstream ss(line);
    float x, y, z;
    ss >> x >> y >> z;
    return { x, y, z };
}

Point3 OBJParser::parse_point3(const std::string& line) {
    return parse_vector3(line);
}

bool OBJParser::starts_with(const std::string& str, const std::string& prefix) {
    return str.rfind(prefix, 0) == 0;
}

SceneConfig OBJParser::parse_scene(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    Scene scene;
    OutputParameters out_params;
    out_params.width = 800;
    out_params.height = 800;
    out_params.output_filename = "output.ppm";
    Color bg_color(0, 0, 0);

    // Geometry Data
    std::vector<Point3> global_vertices;
    std::vector<Vector3> global_normals;
    
    // Mesh State
    std::shared_ptr<Mesh> current_mesh = nullptr;
    std::string current_name = "default";
    bool is_new_object = true;
    
    // Camera State
    Point3 cam_pos(0, 0, 0);
    Point3 cam_lookat(0, 0, -1);
    Vector3 cam_up(0, 1, 0);

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        if (starts_with(line, "v ")) {
            if (is_new_object) {
                is_new_object = false;
                current_mesh = std::make_shared<Mesh>();
            }

            current_mesh->add_vertex(parse_point3(line.substr(2)));
            global_vertices.push_back(parse_point3(line.substr(2)));
        }
        else if (starts_with(line, "vn ")) {
            global_normals.push_back(parse_vector3(line.substr(3)));
        }
        else if (starts_with(line, "usemtl ")) {
            std::string name = line.substr(7);
            name.erase(name.size() - 1); // Remove newline character
            current_name = name;

            if (current_name != "light") {
                // Start a new mesh
                current_mesh->set_name(current_name);
                current_mesh->set_color(get_color_for_material(current_name));
                scene.add_object(current_mesh);
            }

        }
        else if (starts_with(line, "lp ")) {
            is_new_object = true;
            std::stringstream ss(line.substr(3));
            int lp_val;
            ss >> lp_val;

            Point3 origin = global_vertices.back();
            global_vertices.pop_back();
            current_mesh = nullptr;
            scene.add_light(Light(origin, "light", lp_val));
        }
        else if (starts_with(line, "f ")) {
            is_new_object = true;
            std::stringstream ss(line.substr(2));
            int idx[3];
            for (int i = 0; i < 3; ++i) {
                ss >> idx[i];
            }

            Point3 p0 = global_vertices[idx[0] - 1];
            Point3 p1 = global_vertices[idx[1] - 1];
            Point3 p2 = global_vertices[idx[2] - 1];
            
            Triangle tri(p0, p1, p2, current_mesh->get_color());
            current_mesh->add_triangle(tri);
        }
        else if (starts_with(line, "g ")) {
            is_new_object = true;
            // Parse Camera
            if (global_vertices.size() >= 2) {
                cam_lookat = global_vertices.back();
                cam_pos = global_vertices[global_vertices.size() - 2];
            }

            if (!global_normals.empty()) {
                cam_up = global_normals.back();
            }
        }
    }

    float aspect_ratio = static_cast<float>(out_params.width) / static_cast<float>(out_params.height);
    Camera camera = Camera(cam_pos, cam_lookat, cam_up, 70.0f, aspect_ratio);

    return SceneConfig(scene, camera, out_params, bg_color);
}

Color OBJParser::get_color_for_material(const std::string& mtl_name) {
    // Hardcoded colors for standard Cornell Box materials
    if (mtl_name == "floor") return Color(0.7f, 0.7f, 0.7f); // White/Grey
    if (mtl_name == "ceiling") return Color(0.7f, 0.7f, 0.7f);
    if (mtl_name == "back") return Color(0.7f, 0.7f, 0.7f);
    if (mtl_name == "green") return Color(0.12f, 0.45f, 0.15f); // Right wall
    if (mtl_name == "red") return Color(0.65f, 0.05f, 0.05f);   // Left wall
    if (mtl_name == "short_box") return Color(0.0f, 0.0f, 1.0f);
    if (mtl_name == "tall_box") return Color(0.0f, 0.0f, 0.0f);
    if (mtl_name == "light") return Color(10.0f, 10.0f, 10.0f); // Bright emission
    return Color(0.5f, 0.5f, 0.5f); // Default
}

} // namespace PathRender
