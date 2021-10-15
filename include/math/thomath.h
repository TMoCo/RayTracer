//
// Utility include for all math types and struct, plus some constants and utility stuff
//

#ifndef THOMATH_H_
#define THOMATH_H_ 1

#include <core/types.h>

#include <math/Vector2.h>
#include <math/Vector3.h>
#include <math/Vector4.h>

#include <math/Quaternion.h>

#include <math/Matrix4.h>

#include <math/Transform.h>

// OpenGL directions (right handed coordinate system)
constexpr Vector3 UP    {  0.0f,  1.0f,  0.0f };
constexpr Vector3 DOWN  {  0.0f, -1.0f,  0.0f };
constexpr Vector3 LEFT  { -1.0f,  0.0f,  0.0f };
constexpr Vector3 RIGHT {  1.0f,  0.0f,  0.0f };
constexpr Vector3 FRONT {  0.0f,  0.0f, -1.0f }; // camera points in -Z
constexpr Vector3 BACK  {  0.0f,  0.0f,  1.0f };

// math constants
constexpr F32 PI   = 3.14159265358979323846f;
constexpr F32 PI_2 = 1.57079632679489661923f;
constexpr F32 PI_4 = 0.785398163397448309616f;

constexpr F32 EPSILON = 0.00001f;

constexpr F32 RADIANS(F32 x) {
  return x * PI * 0.00555555556f;
}

#endif // !THOMATH_H_
