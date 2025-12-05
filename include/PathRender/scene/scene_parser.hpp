#ifndef PATHRENDER_SCENE_PARSER_HPP_
#define PATHRENDER_SCENE_PARSER_HPP_

#include "PathRender/scene/scene_config.hpp"
#include <string>
#include <yaml-cpp/yaml.h>

namespace PathRender {

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