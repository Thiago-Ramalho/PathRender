#ifndef PATHRENDER_MATH_UTILS_HPP_
#define PATHRENDER_MATH_UTILS_HPP_

#include "PathRender/core/vector.hpp"

using namespace PathRender;

namespace Utils {

  // Helper: Generates a coordinate system (Tangent, Bitangent) given a Normal
  void build_orthonormal_basis(const Vector3& normal, Vector3& tangent, Vector3& bitangent);

} // namespace Utils

#endif // PATHRENDER_MATH_UTILS_HPP_