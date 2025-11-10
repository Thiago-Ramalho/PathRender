# 🎨 PathRender - Path Tracing Engine

A rendering engine based on path tracing to generate realistic images through physical light simulation.

Developed as a project for the Computer Graphics course in the Computer Science graduate program at Centro de Informática - UFPE

## 📁 Project Structure

```
PathRender/
├── CMakeLists.txt           # Main CMake configuration
├── include/                 # Public library headers
│   ├── PathRender.hpp      # Main header (includes everything)
│   └── PathRender/
│       ├── core/           # Fundamental math types
│       │   ├── vector.hpp  # Vector3
│       │   ├── point.hpp   # Point3
│       │   ├── ray.hpp     # Ray
│       │   ├── color.hpp   # Color
│       │   ├── matrix.hpp  # Matrix4x4
│       │   └── material.hpp # Material
│       ├── objects/        # Renderable objects
│       │   ├── sphere.hpp  # Sphere
│       │   ├── plane.hpp   # Plane
│       │   └── objects.hpp # Base Object class
│       └── scene/          # Scene management
│           ├── camera.hpp  # Camera
│           ├── scene.hpp   # Scene (contains objects)
│           └── scene_parser.hpp # YAML Parser
├── src/                    # Library implementation
│   ├── CMakeLists.txt
│   ├── core/              # Core implementations
│   ├── objects/           # Object implementations
│   └── scene/             # Scene implementations
├── app/                   # Demo application
│   ├── CMakeLists.txt
│   └── main.cpp           # Main test program
└── scenes/                # YAML scene files
    └── simple_scene.yml   # Example scene
```

## 🚀 Getting Started

### Requirements
- CMake 3.13+
- C++17 compatible compiler (GCC, Clang, MSVC)

### Building

```bash
# Clone the repository
git clone <repository-url>
cd PathRender

# Build the project
./build.sh

# Or manually:
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Running

```bash
# From project root
./build/bin/pathrender_demo
```

The rendered images will be saved in the `output/` directory with timestamps to avoid overwriting.


## 📝 License

[Define license]
