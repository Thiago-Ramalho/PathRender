#include "PathRender/rendering/PathTracer.hpp"
#include <iostream>
#include <iomanip>

namespace PathRender {

PathTracer::PathTracer() : rng(std::random_device{}()) {}

void PathTracer::render(std::vector<Color>& buffer, const SceneConfig& config) {
    std::cout << "PATH TRACER RENDER" << std::endl;
    const Scene& scene = config.scene;
    const Camera& camera = config.camera;
    const int width = config.output_params.width;
    const int height = config.output_params.height;
    const int number_of_rays = 50;
    const int total_pixels = width * height;
    int pixels_rendered = 0;

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Color pixel_color(0, 0, 0);

            // 1. Anti-Aliasing Loop
            for(int k = 0; k < number_of_rays; k++){    
                // Add random offset for AA
                float u = (float(i) + random()) / (width - 1);
                float v = (float(j) + random()) / (height - 1);
                
                Ray ray = camera.get_ray(u, v);
                pixel_color += trace_path(ray, 0, scene);                
            }

            // Average samples
            pixel_color /= (double)number_of_rays;
            
            // 2. Gamma Correction (Approximate for display)
            // Monitors expect gamma corrected values (power of 1/2.2, or sqrt for approx)
            pixel_color = Color(sqrt(pixel_color.r), sqrt(pixel_color.g), sqrt(pixel_color.b));

            buffer[(height - 1 - j) * width + i] = pixel_color;

            // Atualizar barra de progresso
            ++pixels_rendered;
            if (pixels_rendered % 100 == 0 || pixels_rendered == total_pixels) {
                float progress = (float)pixels_rendered / total_pixels * 100.0f;
                int bar_width = 50;
                int filled = (int)(progress / 100.0f * bar_width);
                
                std::cout << "\rProgresso: [";
                for (int k = 0; k < bar_width; ++k) {
                    if (k < filled) {
                        std::cout << "█";
                    } else {
                        std::cout << "░";
                    }
                }
                std::cout << "] " << std::fixed << std::setprecision(1) << progress << "% (" 
                         << pixels_rendered << "/" << total_pixels << ")";
                std::cout.flush();
            }
        }
    }
    
    // Nova linha após completar o progresso
    std::cout << std::endl;
}

Vector3 PathTracer::random_unit_vector_in_hemisphere_of(const Vector3& normal) {
    Vector3 v;
    while (true) {
        // 1. Gera um vetor aleatório dentro do cubo [-1, 1]
        // random() * 2.0 - 1.0 converte o range [0, 1] para [-1, 1]
        float x = random() * 2.0f - 1.0f;
        float y = random() * 2.0f - 1.0f;
        float z = random() * 2.0f - 1.0f;

        // 2. Calcula o comprimento ao quadrado
        float lenSq = x*x + y*y + z*z;

        // 3. Se estiver dentro da esfera unitária (e não for zero), aceitamos
        if (lenSq > 0.0001f && lenSq <= 1.0f) {
            // Normaliza para virar um vetor unitário (tamanho 1)
            float len = std::sqrt(lenSq);
            v = Vector3(x / len, y / len, z / len);
            break;
        }
    }

    // 4. Garante que está no mesmo hemisfério da normal (Dot Product)
    float dot = v.x * normal.x + v.y * normal.y + v.z * normal.z;
    
    // Se o produto escalar for negativo, aponta para o lado oposto, então invertemos
    if (dot < 0.0f) {
        return Vector3(-v.x, -v.y, -v.z);
    }

    return v;
}

Vector3 PathTracer::RandomUnitVector() {
    float z = random() * 2.0f - 1.0f;
    float a = random() * M_PI * 2.0f;
    float r = sqrt(1.0f - z * z);
    float x = r * cos(a);
    float y = r * sin(a);
    return Vector3(x, y, z);
}
Vector3 PathTracer::mix(const Vector3& x, const Vector3& y, double a){
    return x * (1.0f - a) + y * a;
}

Vector3 PathTracer::reflect(const Vector3& v, const Vector3& n) {
    return v - n * 2.0f * v.dot(n);
}

Vector3 PathTracer::refract(const Vector3& v, const Vector3& n, float eta) {
    float cos_theta = fmin((-v).dot(n), 1.0f);
    Vector3 r_out_perp = (v + n * cos_theta) * eta;
    float r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.length_squared()));
    return r_out_perp + n * r_out_parallel;
}

// Schlick approximation for glass reflectivity at angles
float PathTracer::reflectance(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

Color PathTracer::trace_path(const Ray& ray, int depth, const Scene& scene) {
    if (depth >= max_depth) {
        return Color{};  // Bounced enough times.
    }
    HitRecord hit;
    if (!scene.intersect(ray, 0.001f, 10000000000.0f, hit)) {
        // Return sky color or black        
        return Color{};  // Nothing was hit.
    }

    auto&& material = hit.object->get_material();

    // 3. Emission
    if (material.is_light) {
        // Optimization: If you hit a light, you generally stop tracing or return immediately 
        // if it's a pure light source with no reflection.
        return material.color;
    }

    double diffuseProbability = material.kd;
    double specularProbability = material.ks;
    // double transmitProbability = material.kt;

    Ray scattered_ray;
    Point3 hit_point = ray.origin + ray.direction * hit.t;
    Vector3 normal = hit.normal;

    double total = diffuseProbability + specularProbability;// + transmitProbability;
    double rayProbability = random() * total; 
    if (rayProbability < diffuseProbability) {
        Vector3 direction = (normal + RandomUnitVector()).normalized();
        
        // Catch degenerate rays
        if (direction.length() < 0.001f) {
            direction = normal;
        } 

        scattered_ray = Ray(hit_point + normal * 0.01, direction);
    } else {
        Vector3 reflected = reflect(ray.direction.normalized(), normal);
        
        // Convert 'Shininess' (n) to 'Roughness' (fuzz)
        // Heuristic: n=5 -> fuzz=0.4 (Blurry), n=100 -> fuzz=0.02 (Sharp)
        float fuzz = 2.0f / (material.n); 

        // Clamp fuzz to reasonable limits
        if (fuzz > 1.0f) {
            fuzz = 1.0f;
        }

        // Apply fuzz: Add a random sphere vector to the reflection
        Vector3 direction = (reflected + RandomUnitVector() * fuzz).normalized();
        
        // Catch bad scatters (ray going into the surface)
        if (direction.dot(normal) <= 0) {
            direction = reflected;
        } 

        scattered_ray = Ray(hit_point + normal * 0.01, direction);
    }

    return material.color * trace_path(scattered_ray, depth + 1, scene);
}




} // namespace PathRender
