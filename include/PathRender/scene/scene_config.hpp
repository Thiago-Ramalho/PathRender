#ifndef PATHRENDER_SCENE_CONFIG_HPP_
#define PATHRENDER_SCENE_CONFIG_HPP_

#include "PathRender/scene/scene.hpp"
#include "PathRender/scene/camera.hpp"
#include <string>
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

} // namespace PathRender

#endif // PATHRENDER_SCENE_CONFIG_HPP_