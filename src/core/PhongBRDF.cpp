#include "PathRender/core/PhongBRDF.hpp"

namespace PathRender {

bool PhongBRDF::scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec, std::mt19937& rng) const {
  return true;
}

} // namespace PathRender
