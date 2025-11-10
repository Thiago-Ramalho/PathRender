# Coletar todos os arquivos fonte
file(GLOB_RECURSE PATHRENDER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/objects/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/*.cpp
)

# Criar biblioteca PathRender
add_library(PathRender STATIC ${PATHRENDER_SOURCES})

# Configurar diretórios de include
target_include_directories(PathRender
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Definir macros de build
target_compile_definitions(PathRender PUBLIC
    PATHRENDER_BUILD_CORE
    PATHRENDER_BUILD_OBJECTS
    PATHRENDER_BUILD_SCENE
)

# C++17 ou superior
target_compile_features(PathRender PUBLIC cxx_std_17)

# Opções de compilação
if(MSVC)
    target_compile_options(PathRender PRIVATE /W4)
else()
    target_compile_options(PathRender PRIVATE -Wall -Wextra -pedantic)
endif()
