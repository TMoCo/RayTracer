/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// constants
//

#ifndef CONSTANTS_H
#define CONSTANTS_H 1

#include <core/types.h>
#include <math/Vector3.h>

typedef enum : UI32
{
  X = 0,
  Y = 1,
  Z = 2
} AXIS;

// OpenGL directions (assumes right-handed coordinate system)
constexpr Vector3 UP{ 0.0f,  1.0f,  0.0f };
constexpr Vector3 DOWN{ 0.0f, -1.0f,  0.0f };
constexpr Vector3 LEFT{ -1.0f,  0.0f,  0.0f };
constexpr Vector3 RIGHT{ 1.0f,  0.0f,  0.0f };
constexpr Vector3 FRONT{ 0.0f,  0.0f, -1.0f };
constexpr Vector3 BACK{ 0.0f,  0.0f,  1.0f };

constexpr F32 PI      = 3.141592653589793238463f;
constexpr F32 PI_TWO  = 1.570796326794896619231f;
constexpr F32 PI_FOUR = 0.7853981633974483096157f;
constexpr F32 PI_180  = 0.01745329251994329576924f;
constexpr F32 TWO_PI  = 6.283185307179586476925f;
constexpr F32 FOUR_PI = 12.56637061435917295385f;

constexpr F32 EPSILON = 0.0001f;
constexpr F32 MACHINE_EPSILON = FLT_EPSILON;

#endif // !CONSTANTS_H
