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
#include "PathRender/utils/filesystem_utils.hpp"

using namespace PathRender;
using namespace Utils;

int main(int argc, char** argv) {
    std::cout << "=== PathRender - Ray Caster Demo ===" << std::endl;
    
    // Configurações da imagem
    const int width = 800;
    const int height = 600;
    const float aspect_ratio = static_cast<float>(width) / height;
    
    // Criar cena
    Scene scene;
    
    // Adicionar objetos à cena
    // Esfera vermelha no centro
    auto sphere1 = std::make_shared<Sphere>(
        Point3(0, 0, -5),    // posição
        1.0f,                 // raio
        Color(1.0, 0.0, 0.0)  // cor vermelha
    );
    scene.add_object(sphere1);
    
    // Esfera verde à esquerda
    auto sphere2 = std::make_shared<Sphere>(
        Point3(-2.5, 0.5, -6),
        0.8f,
        Color(0.0, 1.0, 0.0)  // cor verde
    );
    scene.add_object(sphere2);
    
    // Esfera azul à direita
    auto sphere3 = std::make_shared<Sphere>(
        Point3(2, -0.5, -4),
        0.6f,
        Color(0.0, 0.0, 1.0)  // cor azul
    );
    scene.add_object(sphere3);
    
    // Plano (chão) cinza
    auto ground = std::make_shared<Plane>(
        Point3(0, -1, 0),     // ponto no plano
        Vector3(0, 1, 0),     // normal (apontando para cima)
        Color(0.5, 0.5, 0.5)  // cor cinza
    );
    scene.add_object(ground);
    
    // Criar câmera
    Camera camera(
        Point3(0, 1, 0),      // posição da câmera
        Point3(0, 0, -5),     // ponto que a câmera olha
        Vector3(0, 1, 0),     // vetor "up"
        60.0f,                // campo de visão (FOV) em graus
        aspect_ratio          // aspect ratio
    );
    
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
            Color pixel_color(0.2, 0.3, 0.4); // cor de fundo (céu azulado)
            
            if (scene.intersect(ray, 0.001f, 1000.0f, hit)) {
                // Se houver interseção, usar a cor do objeto (sem iluminação)
                pixel_color = hit.color;
            }
            
            // Armazenar pixel (invertendo y para PPM)
            pixels[(height - 1 - j) * width + i] = pixel_color;
        }
    }
    
    std::cout << "Progresso: 100%" << std::endl;
    
    // Garantir que o diretório output existe e gerar nome único
    std::string output_dir = ensure_output_directory();
    std::string timestamp = generate_timestamp();
    std::string filename = output_dir + "/render_" + timestamp + ".ppm";
    
    // Salvar imagem
    save_ppm(filename, width, height, pixels);
    
    std::cout << "=== Renderização completa! ===" << std::endl;
    std::cout << "Abra o arquivo 'output.ppm' com um visualizador de imagens." << std::endl;
    
    return 0;
}
