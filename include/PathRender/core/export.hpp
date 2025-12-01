#ifndef PATHRENDER_EXPORT_HPP_
#define PATHRENDER_EXPORT_HPP_

// Export/import macros for shared libraries (Windows) or visibility control (Unix)
// For this project we can keep it simple; when building a static lib this is a no-op.
#if defined(_WIN32) || defined(_WIN64)
  #if defined(PATHRENDER_BUILD)
    #define PATHRENDER_EXPORT __declspec(dllexport)
  #else
    #define PATHRENDER_EXPORT __declspec(dllimport)
  #endif
#else
  #define PATHRENDER_EXPORT
#endif

#endif // PATHRENDER_EXPORT_HPP_