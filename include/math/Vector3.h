/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// Vector3 class declaration
//

#ifndef VECTOR3_H_
#define VECTOR3_H_ 1

#include <core/types.h>

#include <iostream>
#include <xmmintrin.h>

constexpr UI32 SIZEOF_VEC3 = 0xc;

typedef struct Vector3 {
  union {
    F32 _v[4]; // still in fact 4 for simd operations (4th element is ignored in computation)

    __m128 __v;

    struct {
      F32 x, y, z; 
    };
  };

  constexpr Vector3() : _v{} {}

  constexpr Vector3(F32 value) : _v{ value, value, value } {}

  constexpr Vector3(F32 X, F32 Y, F32 Z) : _v{ X, Y, Z } {}

  constexpr Vector3(const F32* values) : _v{ values[0], values[1], values[2] } {}

  constexpr Vector3(const Vector3& other) : _v{ other.x, other.y, other.z } {}

  // binary operators
  Vector3& operator =(const Vector3& other);

  bool operator ==(const Vector3& other) const;

  Vector3& operator +=(const Vector3& other);

  Vector3& operator -=(const Vector3& other);

  Vector3& operator /=(const Vector3& other);

  Vector3& operator *=(const Vector3& other);

  Vector3& operator /=(const F32& other);

  Vector3& operator *=(const F32& factor);

  friend inline Vector3 operator +(Vector3 lhs, const Vector3& rhs) {
      return lhs += rhs;
  }

  friend inline Vector3 operator -(Vector3 lhs, const Vector3& rhs) {
      return lhs -= rhs;
  }

  friend inline Vector3 operator /(Vector3 lhs, const Vector3& rhs) {
      return lhs /= rhs;
  }

  friend inline Vector3 operator *(Vector3 lhs, const Vector3& rhs) {
      return lhs *= rhs;
  }

  Vector3 operator -() const;

  F32& operator [](const UI32 index);

  const F32& operator [](const UI32 index) const;
    
  F32 dot(const Vector3& other) const;

  Vector3 cross(const Vector3& other) const;

  F32 length() const;

  F32 lengthSquared() const;

  Vector3 normalize() const;

  static Vector3 reflect(const Vector3& v, const Vector3& normal);

  static Vector3 refract(const Vector3& v, const Vector3& normal, const F32& iorRatio);
} Vector3;

// binary operators
Vector3 operator /(Vector3 lhs, const F32& rhs);

Vector3 operator /(const F32 lhs, Vector3& rhs);

Vector3 operator *(const F32& lhs, Vector3 rhs);

Vector3 operator *(Vector3 lhs, const F32& rhs);

// stream operators
std::istream& operator >> (std::istream& inStream, Vector3& vector);

std::ostream& operator << (std::ostream& outStream, const Vector3& vector);

#endif // ! VECTOR3_H_
