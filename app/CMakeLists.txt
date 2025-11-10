# Aplicação demo do PathRender
add_executable(pathrender_demo main.cpp)

target_link_libraries(pathrender_demo PRIVATE PathRender)

# Copiar executável para o diretório de saída
set_target_properties(pathrender_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
