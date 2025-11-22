#include "PathRender/utils/filesystem_utils.hpp"
#include "PathRender/scene/scene_parser.hpp"
#include <chrono>
#include <iomanip>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace Utils {

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

} // namespace Utils