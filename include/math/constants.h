/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// constants
//

#ifndef CONSTANTS_H_
#define CONSTANTS_H_ 1

#include <core/types.h>
#include <math/Vector3.h>

typedef enum : UI32
{
  X = 0,
  Y = 1,
  Z = 2
} AXIS;

// OpenGL directions (right handed coordinate system)
constexpr Vector3 UP{ 0.0f,  1.0f,  0.0f };
constexpr Vector3 DOWN{ 0.0f, -1.0f,  0.0f };
constexpr Vector3 LEFT{ -1.0f,  0.0f,  0.0f };
constexpr Vector3 RIGHT{ 1.0f,  0.0f,  0.0f };
constexpr Vector3 FRONT{ 0.0f,  0.0f, -1.0f }; // camera points in -Z
constexpr Vector3 BACK{ 0.0f,  0.0f,  1.0f };

// math constants
constexpr F32 PI = 3.14159265358979323846f;
constexpr F32 PI_2 = 1.57079632679489661923f;
constexpr F32 PI_4 = 0.785398163397448309616f;
constexpr F32 EPSILON = 0.00001f;

#endif // !CONSTANTS_H_
