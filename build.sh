#!/bin/bash
# Script de build para PathRender

set -e  # Para em caso de erro

echo "=== PathRender Build Script ==="
echo ""

# Cores para output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Criar diretório de build
if [ ! -d "build" ]; then
    echo -e "${YELLOW}Criando diretório build...${NC}"
    mkdir build
fi

cd build

# Configurar CMake
echo -e "${YELLOW}Configurando CMake...${NC}"
cmake ..

# Compilar
echo -e "${YELLOW}Compilando...${NC}"
make -j$(nproc)

# Verificar sucesso
if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}✓ Compilação bem sucedida!${NC}"
    echo ""
    echo "Executável criado em: build/bin/pathrender_demo"
    echo ""
    echo "Para executar:"
    echo "  cd build"
    echo "  ./bin/pathrender_demo"
    echo ""
else
    echo ""
    echo -e "${RED}✗ Erro na compilação${NC}"
    exit 1
fi
