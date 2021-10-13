//
// Some core preprocessor directives and constants
//

#ifndef CORE_H
#define CORE_H 1

#include <chrono>
typedef  std::chrono::system_clock sys_clock;

#include <cmath>

constexpr float PI = 3.14159265358979323846f;
constexpr float PI_2 = 1.57079632679489661923f;
constexpr float PI_4 = 0.785398163397448309616f;

// math utils
#define RADIANS(x) \
    x * PI / 180.0f

#define NO_COPY(Type) \
    Type(const Type& T) = delete; \
    Type& operator=(const Type& T) = delete;

// application constants
#define SIZEOF_MAT4 0x40
#define SIZEOF_VEC4 0x10
#define SIZEOF_VEC3 0xc
#define SIZEOF_VEC2 0x8

#define MAX_LINE 1024

#define EPSILON 0.00001f // smaller than this is clamped to 0

#define MAX_NAME_LENGTH 0x80 // todo: remove

#endif // CORE_H