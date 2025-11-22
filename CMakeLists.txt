cmake_minimum_required(VERSION 3.13.0)

project(PathRender VERSION 0.0.1 LANGUAGES C CXX)

find_package(yaml-cpp REQUIRED)

# C++17 como padrão
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Diretório de saída dos executáveis
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Adicionar submódulos
add_subdirectory(src)    # Biblioteca PathRender
add_subdirectory(app)    # Aplicação demo