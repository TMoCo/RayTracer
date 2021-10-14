//
// Vector3 class declaration
//

#ifndef VECTOR3_H
#define VECTOR3_H 1

#include <core/types.h>

#include <iostream>
#include <xmmintrin.h>

constexpr UI32 SIZEOF_VEC3 = 0xc;

typedef struct Vec3 {
  union {
    F32 _v[4]; // still in fact 4 for simd operations (4th element is zeroe ignored)
    __m128 __v;
    struct {
      F32 x, y, z; 
    };
  };

  constexpr Vec3() : _v{} {}

  constexpr Vec3(F32 value) : _v{ value, value, value } {}

  constexpr Vec3(F32 X, F32 Y, F32 Z) : _v{ X, Y, Z } {}

  Vec3(const F32* values) : _v{} { memcpy(_v, values, SIZEOF_VEC3); }

  Vec3(const Vec3& other) : _v{} { memcpy(_v, other._v, SIZEOF_VEC3); }

  // binary operators
  Vec3& operator =(const Vec3& other);

  bool operator ==(const Vec3& other) const;

  Vec3& operator +=(const Vec3& other);

  Vec3& operator -=(const Vec3& other);

  Vec3& operator /=(const Vec3& other);

  Vec3& operator *=(const Vec3& other);

  Vec3& operator /=(const F32& other);

  Vec3& operator *=(const F32& factor);

  friend inline Vec3 operator +(Vec3 lhs, const Vec3& rhs) {
      return lhs += rhs;
  }

  friend inline Vec3 operator -(Vec3 lhs, const Vec3& rhs) {
      return lhs -= rhs;
  }

  friend inline Vec3 operator /(Vec3 lhs, const Vec3& rhs) {
      return lhs /= rhs;
  }

  friend inline Vec3 operator *(Vec3 lhs, const Vec3& rhs) {
      return lhs *= rhs;
  }

  Vec3 operator -() const;

  F32& operator [](const UI32 index);

  const F32& operator [](const UI32 index) const;
    
  F32 dot(const Vec3& other) const;

  Vec3 cross(const Vec3& other) const;

  F32 length() const;

  F32 lengthSquared() const;

  Vec3 normalize() const;

  static Vec3 reflect(const Vec3& v, const Vec3& normal);

  static Vec3 refract(const Vec3& v, const Vec3& normal, const float& iorRatio);
} Vector3;

// binary operators
Vector3 operator /(Vector3 lhs, const F32& rhs);

Vector3 operator *(const F32& lhs, Vector3 rhs);

Vector3 operator *(Vector3 lhs, const F32& rhs);

// stream operators
std::istream& operator >> (std::istream& inStream, Vector3& vector);

std::ostream& operator << (std::ostream& outStream, const Vector3& vector);

#endif // ! VECTOR3_H