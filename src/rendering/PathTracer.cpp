#include "PathRender/rendering/PathTracer.hpp"
#include <iostream>
#include <iomanip>

namespace PathRender {

void PathTracer::render(std::vector<Color>& buffer, const SceneConfig& config) {
    std::cout << "MULTI-THREADED PATH TRACER RENDER" << std::endl;
    // std::cout << "SCENE CONFIG" << std::endl;
    // std::cout << config.to_string() << std::endl;
    
    const Scene& scene = config.scene;
    
    // Extract light points for direct lighting (only if enabled)
    if (m_direct_lighting_enabled) {
        extract_light_points(scene);
        std::cout << "Found " << m_light_points.size() << " light sources for direct lighting" << std::endl;
    } else {
        std::cout << "Direct lighting disabled - using pure Monte Carlo" << std::endl;
    }
    const Camera& camera = config.camera;
    const int width = config.output_params.width;
    const int height = config.output_params.height;
    const int number_of_rays = 100;
    
    // Thread management variables
    const int num_threads = 8;
    std::vector<std::thread> threads;
    int rows_per_thread = height / num_threads;
    
    // Atomic counter for progress bar (safe to increment from multiple threads)
    std::atomic<int> pixels_rendered{0};
    int total_pixels = width * height;
    std::mutex print_mutex; // To prevent garbled console output

    // The function that each thread will run
    auto render_chunk = [&](int start_row, int end_row, int thread_id) {
        // 1. Create a LOCAL Random Number Generator for this thread
        // We seed it with random_device + thread_id to ensure unique sequences
        std::mt19937 thread_rng(std::random_device{}() + thread_id);
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        for (int j = start_row; j < end_row; ++j) {
            for (int i = 0; i < width; ++i) {
                Color pixel_color(0, 0, 0);

                // Anti-Aliasing Loop
                for(int k = 0; k < number_of_rays; k++){    
                    // Use local thread_rng
                    float u = (float(i) + dist(thread_rng)) / (width - 1);
                    float v = (float(j) + dist(thread_rng)) / (height - 1);
                    
                    Ray ray = camera.get_ray(u, v);
                    // Pass the local RNG down the chain
                    pixel_color += trace_path(ray, 0, scene, thread_rng);                
                }

                pixel_color /= (double)number_of_rays;
                pixel_color = Color(sqrt(pixel_color.r), sqrt(pixel_color.g), sqrt(pixel_color.b));
                
                // Write to buffer (Thread safe because each thread writes to unique indices)
                buffer[(height - 1 - j) * width + i] = pixel_color;

                // Progress Bar Logic
                int completed = ++pixels_rendered; // Atomic increment
                
                // Only print every 100 pixels to save console IO time
                if (completed % 100 == 0 || completed == total_pixels) {
                    // Try to lock. If busy, just skip printing this frame (optimization)
                    if (print_mutex.try_lock()) {
                        float progress = (float)completed / total_pixels * 100.0f;
                        std::cout << "\rProgress: " << std::fixed << std::setprecision(1) << progress << "%   ";
                        std::cout.flush();
                        print_mutex.unlock();
                    }
                }
            }
        }
    };

    // 2. Launch Threads
    for (int t = 0; t < num_threads; ++t) {
        int start_row = t * rows_per_thread;
        int end_row = (t == num_threads - 1) ? height : (t + 1) * rows_per_thread;
        
        // Emplace_back creates and starts the thread
        threads.emplace_back(render_chunk, start_row, end_row, t);
    }

    // 3. Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "\nRender Complete!" << std::endl;
}

Vector3 PathTracer::random_unit_vector_in_hemisphere_of(const Vector3& normal, std::mt19937& thread_rng) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    Vector3 v;
    while (true) {
        // 1. Gera um vetor aleatório dentro do cubo [-1, 1]
        // random() * 2.0 - 1.0 converte o range [0, 1] para [-1, 1]
        float x = dist(thread_rng) * 2.0f - 1.0f;
        float y = dist(thread_rng) * 2.0f - 1.0f;
        float z = dist(thread_rng) * 2.0f - 1.0f;

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
    // Se o produto escalar for negativo, aponta para o lado oposto, então invertemos
    if (v.dot(normal) < 0.0f) {
        return Vector3(-v.x, -v.y, -v.z);
    }

    return v;
}

Vector3 PathTracer::mix(const Vector3& x, const Vector3& y, double a){
    return x * (1.0f - a) + y * a;
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

Color PathTracer::trace_path(const Ray& ray, int depth, const Scene& scene, std::mt19937& thread_rng) {
    if (depth >= max_depth) {
        return Color{};  // Bounced enough times.
    }
    HitRecord hit;
    if (!scene.intersect(ray, 0.001f, 10000000000.0f, hit)) {
        // Return sky color or black        
        return Color{};  // Nothing was hit.
    }

    auto&& material = hit.object->get_material();
    Point3 hit_point = ray.origin + ray.direction * hit.t;
    Vector3 normal = hit.normal;

    // 3. Emission - if we hit a light source
    if (material.is_light) {
        return material.brdf->color;
    }

    // 4. Calculate direct lighting contribution (if enabled)
    Color direct_light(0, 0, 0);
    if (m_direct_lighting_enabled) {
        direct_light = calculate_direct_lighting(hit_point, normal, material, scene, thread_rng);
    }

    // 5. Monte Carlo indirect lighting (existing logic)
    Color indirect_light(0, 0, 0);
    ScatterRecord srec;
    if (material.brdf->scatter(ray, hit, srec, thread_rng)) {
        indirect_light = srec.attenuation * trace_path(srec.out_ray, depth + 1, scene, thread_rng);
    }

    // 6. Combine direct and indirect lighting
    return direct_light + indirect_light;
}

void PathTracer::extract_light_points(const Scene& scene) {
    m_light_points.clear();
    
    const auto& objects = scene.get_objects();
    for (const auto& obj : objects) {
        const Material& material = obj->get_material();
        if (material.is_light) {
            // For mesh lights, sample multiple points
            // For now, use object center as single light point
            Point3 light_pos = obj->get_position();
            Color light_color = material.brdf->color;
            float intensity = (light_color.r + light_color.g + light_color.b) / 3.0f;
            
            m_light_points.push_back({light_pos, light_color, intensity});
        }
    }
}

Color PathTracer::calculate_direct_lighting(const Point3& hit_point, const Vector3& normal, 
                                           const Material& material, const Scene& scene, 
                                           std::mt19937& /*thread_rng*/) {
    Color total_light(0, 0, 0);
    
    // Sample all light sources
    for (const auto& light : m_light_points) {
        // Vector from hit point to light
        Vector3 light_dir = (light.position - hit_point).normalized();
        float distance = (light.position - hit_point).length();
        
        // Check if light is above surface (dot product with normal)
        float n_dot_l = normal.dot(light_dir);
        if (n_dot_l <= 0) continue;  // Light behind surface
        
        // Shadow test
        if (is_in_shadow(hit_point, light.position, scene)) {
            continue;  // In shadow, skip this light
        }
        
        // Calculate light attenuation (inverse square law)
        float attenuation = 1.0f / (1.0f + 0.001f * distance * distance);
        
        // Simple Lambertian diffuse lighting
        Color diffuse_contribution = material.brdf->color * light.color * n_dot_l * attenuation * light.intensity;
        
        total_light += diffuse_contribution;
    }
    
    return total_light * 0.8f;  // Scale by diffuse coefficient (assuming kd=0.8)
}

bool PathTracer::is_in_shadow(const Point3& point, const Point3& light_pos, const Scene& scene) {
    Vector3 light_dir = (light_pos - point).normalized();
    float light_distance = (light_pos - point).length();
    
    // Create shadow ray (offset slightly to avoid self-intersection)
    Ray shadow_ray(point + light_dir * 0.001f, light_dir);
    
    HitRecord shadow_hit;
    if (scene.intersect(shadow_ray, 0.001f, light_distance - 0.001f, shadow_hit)) {
        // Check if the hit object is the light itself
        if (shadow_hit.object->get_material().is_light) {
            return false;  // Hit the light, not in shadow
        }
        return true;  // Hit something else, in shadow
    }
    
    return false;  // Nothing between point and light
}

} // namespace PathRender
