#ifndef PATHRENDER_SCENE_PARSER_HPP_
#define PATHRENDER_SCENE_PARSER_HPP_

#include "PathRender/objects/plane.hpp"
#include "PathRender/objects/sphere.hpp"
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
    std::string to_string() const;

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
    std::string to_string() const;
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
    virtual ~SceneParser() = default;

    /**
     * @brief Carrega uma cena de um arquivo YAML
     * @param filename Caminho para o arquivo YAML
     * @return Configuração da cena carregada
     * @throws std::runtime_error se o arquivo não puder ser lido
     */
    virtual SceneConfig parse(const std::string& filename) = 0;
};

} // namespace PathRender

#endif // PATHRENDER_SCENE_PARSER_HPP_