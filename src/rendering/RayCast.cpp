#include "PathRender/rendering/RayCast.hpp"

namespace PathRender {

void RayCast::render(std::vector<Color>& buffer, const SceneConfig& config) {
    const Scene& scene = config.scene;
    const Camera& camera = config.camera;
    const int width = config.output_params.width;
    const int height = config.output_params.height;
    const Color& background_color = config.background_color;
    std::cout << "Renderizando cena (" << width << "x" << height << ")..." << std::endl;
    
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
            
            if (scene.intersect(ray, 0.001f, 10000000000.0f, hit)) {
                // Se houver interseção, usar a cor do objeto (sem iluminação)
                pixel_color = hit.object->get_material().color;
            }
            
            // Armazenar pixel (invertendo y para PPM)
            buffer[(height - 1 - j) * width + i] = pixel_color;
        }
    }
    
    std::cout << "Progresso: 100%" << std::endl;
}

} // namespace PathRender