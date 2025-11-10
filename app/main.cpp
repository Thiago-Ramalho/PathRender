#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include "PathRender/core/vector.hpp"
#include "PathRender/core/point.hpp"
#include "PathRender/core/ray.hpp"
#include "PathRender/core/color.hpp"
#include "PathRender/scene/camera.hpp"
#include "PathRender/scene/scene.hpp"
#include "PathRender/objects/sphere.hpp"
#include "PathRender/objects/plane.hpp"

using namespace PathRender;

// Função para gerar timestamp para nome do arquivo
std::string generate_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
    oss << "_" << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

// Função para garantir que o diretório output existe
std::string ensure_output_directory() {
    // Detectar se estamos executando de build/bin/ e ir para a raiz do projeto
    std::filesystem::path current_dir = std::filesystem::current_path();
    std::filesystem::path project_root;
    
    // Se estivermos em build/bin/, subir duas pastas para chegar à raiz
    if (current_dir.filename() == "bin" && 
        current_dir.parent_path().filename() == "build") {
        project_root = current_dir.parent_path().parent_path();
    } else {
        // Se não estivermos em build/bin/, assumir que estamos na raiz
        project_root = current_dir;
    }
    
    // Criar pasta output na raiz do projeto
    std::filesystem::path output_dir = project_root / "output";
    
    // Criar o diretório se não existir
    if (!std::filesystem::exists(output_dir)) {
        std::filesystem::create_directories(output_dir);
        std::cout << "Diretório criado: " << output_dir << std::endl;
    }
    
    return output_dir.string();
}

// Função para salvar imagem PPM
void save_ppm(const std::string& filename, int width, int height, 
              const std::vector<Color>& pixels) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao criar arquivo: " << filename << std::endl;
        return;
    }
    
    // Cabeçalho PPM
    file << "P3\n" << width << " " << height << "\n255\n";
    
    // Pixels (RGB)
    for (const auto& pixel : pixels) {
        int r = static_cast<int>(255.999 * pixel.r);
        int g = static_cast<int>(255.999 * pixel.g);
        int b = static_cast<int>(255.999 * pixel.b);
        
        // Clamp valores entre 0-255
        r = std::max(0, std::min(255, r));
        g = std::max(0, std::min(255, g));
        b = std::max(0, std::min(255, b));
        
        file << r << " " << g << " " << b << "\n";
    }
    
    file.close();
    std::cout << "Imagem salva em: " << filename << std::endl;
}

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
