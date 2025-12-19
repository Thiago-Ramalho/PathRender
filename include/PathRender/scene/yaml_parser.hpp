#ifndef PATHRENDER_YAML_PARSER_HPP_
#define PATHRENDER_YAML_PARSER_HPP_

#include "PathRender/objects/mesh.hpp"
#include "PathRender/objects/plane.hpp"
#include "PathRender/objects/sphere.hpp"
#include "PathRender/scene/scene_parser.hpp"
#include "PathRender/core/AnisotropicMatteBRDF.hpp"
#include "PathRender/core/PhongBRDF.hpp"
#include "PathRender/core/DieletricBRDF.hpp"

namespace PathRender {

// class YAMLParser : public SceneParser {
// public:
//   YAMLParser() = default;

//   SceneConfig parse(const std::string& filename) override;

// protected:
//   OutputParameters parse_output(const YAML::Node& output_node);
//   Camera parse_camera(const YAML::Node& camera_node, const OutputParameters& output_params);
//   Scene parse_objects(const YAML::Node& objects_node);
//   Color parse_background(const YAML::Node& background_node);
//   std::shared_ptr<Sphere> parse_sphere(const YAML::Node& node);
//   std::shared_ptr<Plane> parse_plane(const YAML::Node& node);
//   Point3 parse_point3(const YAML::Node& node);
//   Vector3 parse_vector3(const YAML::Node& node);
// };

class YAMLParser : public SceneParser {
public:
  YAMLParser() = default;
  SceneConfig parse(const std::string& filename) override;

protected:
  // --- New / Updated Methods ---
  Material parse_material(const YAML::Node& material_node);
  std::shared_ptr<Mesh> parse_quad(const YAML::Node& node); // Returns 2 triangles
  
  // --- Existing Methods (Updated Signatures where needed) ---
  OutputParameters parse_output(const YAML::Node& output_node);
  Camera parse_camera(const YAML::Node& camera_node, const OutputParameters& output_params);
  Scene parse_objects(const YAML::Node& objects_node);
  Color parse_background(const YAML::Node& background_node);
  
  std::shared_ptr<Sphere> parse_sphere(const YAML::Node& node);
  std::shared_ptr<Plane> parse_plane(const YAML::Node& node);
  
  Point3 parse_point3(const YAML::Node& node);
  Vector3 parse_vector3(const YAML::Node& node);
};

} // namespace PathRender

#endif // PATHRENDER_YAML_PARSER_HPP_
