#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include "PathRender/core/vector.hpp"
#include "PathRender/core/point.hpp"
#include "PathRender/core/ray.hpp"
#include "PathRender/core/color.hpp"
#include "PathRender/scene/camera.hpp"
#include "PathRender/scene/scene.hpp"
#include "PathRender/objects/sphere.hpp"
#include "PathRender/objects/plane.hpp"
#include "PathRender/scene/scene_parser.hpp"
#include "PathRender/utils/filesystem_utils.hpp"

using namespace PathRender;
using namespace Utils;

std::vector<Color> render_scene(SceneConfig config) {
    const Scene& scene = config.scene;
    const Camera& camera = config.camera;
    const int width = config.output_params.width;
    const int height = config.output_params.height;
    const Color& background_color = config.background_color;
    std::cout << "Renderizando cena (" << width << "x" << height << ")..." << std::endl;
    
    // Buffer de pixels
    std::vector<Color> pixels(width * height);
    
    // Renderizar (ray casting simples)
    for (int j = 0; j < height; ++j) {
        // Progresso
        if (j % 50 == 0) {
            std::cout << "Progresso: " << (100 * j / height) << "%" << std::endl;
        }
        
        for (int i = 0; i < width; ++i) {
            // Coordenadas normalizadas [0, 1]
            float u = static_cast<float>(i) / (width - 1);
            float v = static_cast<float>(j) / (height - 1);
            
            // Gerar raio da câmera
            Ray ray = camera.get_ray(u, v);
            
            // Testar interseção com a cena
            HitRecord hit;
            Color pixel_color = background_color; // cor de fundo (céu azulado)
            
            if (scene.intersect(ray, 0.001f, 1000.0f, hit)) {
                // Se houver interseção, usar a cor do objeto (sem iluminação)
                pixel_color = hit.color;
            }
            
            // Armazenar pixel (invertendo y para PPM)
            pixels[(height - 1 - j) * width + i] = pixel_color;
        }
    }
    
    std::cout << "Progresso: 100%" << std::endl;
    return pixels;
}

std::string get_scene_filename_from_args(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        // Procurou flag "--scene"
        if (arg == "--scene" && i + 1 < argc) {
            return argv[i + 1];  // pega o nome informado
        }
    }

    throw std::runtime_error("Usage: ./PathRender --scene nome.yaml");
}


int main(int argc, char** argv) {
    std::cout << "=== PathRender - Ray Caster Demo ===" << std::endl;

    std::string scene_filename = get_scene_filename_from_args(argc, argv);
    
    std::filesystem::path exe_path = std::filesystem::canonical(argv[0]).parent_path();
    auto project_root = exe_path.parent_path().parent_path();
    auto scene_path = project_root / "scenes" / scene_filename;

    if (!std::filesystem::exists(scene_path)) {
        std::cerr << "Scene file not found: " << scene_path << std::endl;
        return 1;
    }

    // Parsear configuração da cena
    SceneParser parser;
    SceneConfig config = parser.parse(scene_path.string());
    
    // Renderizar cena com raycast simples
    std::vector<Color> pixels = render_scene(config);
    
    // Garantir que o diretório output existe e gerar nome único
    std::string output_dir = ensure_output_directory();
    std::string timestamp = generate_timestamp();
    std::string filename = output_dir + "/render_" + timestamp + ".ppm";
    
    // Salvar imagem
    save_ppm(filename, config.output_params.width, config.output_params.height, pixels);
    
    std::cout << "=== Renderização completa! ===" << std::endl;
    std::cout << "Abra o arquivo 'output.ppm' com um visualizador de imagens." << std::endl;
    
    return 0;
}
