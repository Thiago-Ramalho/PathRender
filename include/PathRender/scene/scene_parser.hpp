#ifndef A345DE02_764E_4675_8EC5_0F459C9310DC
#define A345DE02_764E_4675_8EC5_0F459C9310DC
#include "PathRender/objects/plane.hpp"
#include "PathRender/objects/sphere.hpp"
#ifndef PATHRENDER_SCENE_PARSER_HPP_
#define PATHRENDER_SCENE_PARSER_HPP_

#include "PathRender/scene/scene.hpp"
#include "PathRender/scene/camera.hpp"
#include <string>
#include <memory>
#include <yaml-cpp/yaml.h>

namespace PathRender {

/**
 * @struct OutputParameters
 * @brief Configuração do output da renderização
 */
struct OutputParameters {
    int width;
    int height;
    std::string output_filename;
};

/**
 * @struct SceneConfig
 * @brief Configuração completa de uma cena carregada de arquivo
 */
struct SceneConfig {
    Scene scene;
    Camera camera;
    OutputParameters output_params;
    Color background_color;

    SceneConfig(const Scene& scene, const Camera& camera, const OutputParameters& output_params, const Color& background_color);
};

/**
 * @class SceneParser
 * @brief Parser para arquivos de cena YAML
 * 
 * @note Esta classe requer a biblioteca yaml-cpp
 * Por enquanto, esta é apenas uma interface - implementação futura
 */
class SceneParser {
public:
    /**
     * @brief Carrega uma cena de um arquivo YAML
     * @param filename Caminho para o arquivo YAML
     * @return Configuração da cena carregada
     * @throws std::runtime_error se o arquivo não puder ser lido
     */
    static SceneConfig parse(const std::string& filename);
    
private:
    static OutputParameters parse_output(const YAML::Node& output_node);
    static Camera parse_camera(const YAML::Node& camera_node, const OutputParameters& output_params);
    static Scene parse_objects(const YAML::Node& objects_node);
    static Color parse_background(const YAML::Node& background_node);
    static Point3 parse_point3(const YAML::Node& node);
    static Vector3 parse_vector3(const YAML::Node& node);
    static std::shared_ptr<Sphere> parse_sphere(const YAML::Node& node);
    static std::shared_ptr<Plane> parse_plane(const YAML::Node& node);
};

} // namespace PathRender

#endif // PATHRENDER_SCENE_PARSER_HPP_


#endif /* A345DE02_764E_4675_8EC5_0F459C9310DC */
