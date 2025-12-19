#include "PathRender/utils/math_utils.hpp"

namespace Utils {
  
void build_orthonormal_basis(const Vector3& normal, Vector3& tangent, Vector3& bitangent) {
      // Pick an arbitrary helper vector that isn't parallel to the normal
      Vector3 helper = (std::abs(normal.x) > 0.9f) ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
      
      // Use Cross Products to build the basis
      bitangent = normal.cross(helper).normalized();
      tangent = bitangent.cross(normal).normalized();
  }

} // namespace Utils