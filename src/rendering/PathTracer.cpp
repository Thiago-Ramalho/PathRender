#include "PathRender/rendering/PathTracer.hpp"
#include <iostream>
#include <iomanip>

namespace PathRender {

void PathTracer::render(std::vector<Color>& buffer, const SceneConfig& config) {
    std::cout << "MULTI-THREADED PATH TRACER RENDER (4 Threads)" << std::endl;
    
    const Scene& scene = config.scene;
    const Camera& camera = config.camera;
    const int width = config.output_params.width;
    const int height = config.output_params.height;
    const int number_of_rays = 100;
    
    // Thread management variables
    const int num_threads = 4;
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

    // 3. Emission
    if (material.is_light) {
        // Optimization: If you hit a light, you generally stop tracing or return immediately 
        // if it's a pure light source with no reflection.
        return material.brdf->color;
    }

    ScatterRecord srec;
    if (material.brdf->scatter(ray, hit, srec, thread_rng)) {
        return srec.attenuation * trace_path(srec.out_ray, depth + 1, scene, thread_rng);
    } else {
        return Color{}; // Absorbed
    }
}

} // namespace PathRender
