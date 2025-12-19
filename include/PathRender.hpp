#ifdef PATHRENDER_BUILD_CORE
#include "PathRender/core/color.hpp"
#include "PathRender/core/material.hpp"
#include "PathRender/core/matrix.hpp"
#include "PathRender/core/point.hpp"
#include "PathRender/core/ray.hpp"
#include "PathRender/core/vector.hpp"
#include "PathRender/core/light.hpp"
#include "PathRender/core/HitRecord.hpp"
#include "PathRender/core/ScatterRecord.hpp"
#include "PathRender/core/BRDF.hpp"
#include "PathRender/core/PhongBRDF.hpp"
#include "PathRender/core/DieletricBRDF.hpp"
#endif // PRISM_CORE

#ifdef PATHRENDER_BUILD_OBJECTS
#include "PathRender/objects/objects.hpp"
#include "PathRender/objects/plane.hpp"
#include "PathRender/objects/sphere.hpp"
#include "PathRender/objects/mesh.hpp"
#include "PathRender/objects/triangle.hpp"
#endif // PATHRENDER_BUILD_OBJECTS

#ifdef PATHRENDER_BUILD_SCENE
#include "PathRender/scene/camera.hpp"
#include "PathRender/scene/scene.hpp"
#include "PathRender/scene/scene_parser.hpp"
#endif

#ifdef PATHRENDER_BUILD_UTILS
#include "PathRender/utils/filesystem_utils.hpp"
#endif // PATHRENDER_BUILD_UTILS