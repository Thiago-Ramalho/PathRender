#ifndef B221C4AC_5E55_4D2B_80B4_D05F113FC0F7
#define B221C4AC_5E55_4D2B_80B4_D05F113FC0F7
#ifndef PATHRENDER_FILESYSTEM_UTILS_HPP_
#define PATHRENDER_FILESYSTEM_UTILS_HPP_

#include <sstream>
#include <vector>
#include "PathRender/core/color.hpp"

using namespace PathRender;

namespace Utils {

  std::string generate_timestamp();

  std::string ensure_output_directory();

  void save_ppm(const std::string& filename, int width, int height, 
              const std::vector<Color>& pixels);

} // namespace Utils

#endif // PATHRENDER_OBJECTS_HPP_

#endif /* B221C4AC_5E55_4D2B_80B4_D05F113FC0F7 */
