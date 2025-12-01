#ifndef F2F0351A_CAA1_478F_A39F_DF52B23A080D
#define F2F0351A_CAA1_478F_A39F_DF52B23A080D
#ifndef PATHRENDER_OBJ_PARSER_HPP_
#define PATHRENDER_OBJ_PARSER_HPP_

#include "PathRender/objects/mesh.hpp"
#include "PathRender/scene/scene_parser.hpp"

namespace PathRender {

class OBJParser : public SceneParser {
public:
  OBJParser() = default;

  SceneConfig parse(const std::string& filename) override;

protected:  
  Vector3 parse_vector3(const std::string& line);
  Point3 parse_point3(const std::string& line);
  bool starts_with(const std::string& str, const std::string& prefix);

  SceneConfig parse_scene(const std::string& filename);
  Color get_color_for_material(const std::string& mtl_name);
};

} // namespace PathRender

#endif // PATHRENDER_OBJ_PARSER_HPP_

#endif /* F2F0351A_CAA1_478F_A39F_DF52B23A080D */
