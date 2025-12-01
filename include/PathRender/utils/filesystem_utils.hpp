#ifndef PATHRENDER_FILESYSTEM_UTILS_HPP_
#define PATHRENDER_FILESYSTEM_UTILS_HPP_

#include <sstream>
#include <vector>
#include "PathRender/scene/scene_parser.hpp"
#include "PathRender/core/color.hpp"

using namespace PathRender;

namespace Utils {

  std::string generate_timestamp();

  std::string ensure_output_directory();

  void save_ppm(const std::string& filename, int width, int height, 
              const std::vector<Color>& pixels);

} // namespace Utils

#endif // PATHRENDER_OBJECTS_HPP_