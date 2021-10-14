//
// Vector4 class declaration
//

#ifndef VECTOR4_H
#define VECTOR4_H 1

#include <core/types.h>

#include <math/Vector3.h> // for Homogeneous vectors (could move to Vectors.h in separate class...)

#include <string.h>
#include <xmmintrin.h>

constexpr UI32 SIZEOF_VEC4 = 0x10;

typedef struct Vector4 {
public:
  union {
    F32     _v[4];
    __m128 __v;
    struct {
      F32 x, y, z, w;
    };
  };

  constexpr Vector4() :_v{} {}

  constexpr Vector4(F32 X, F32 Y, F32 Z, F32 W = 1.0f) : _v{ X, Y, Z, W } {}

  Vector4(const F32* values) : _v{} { memcpy(_v, values, SIZEOF_VEC4); }

  Vector4(const Vector4& other) : _v{} {memcpy(_v, other._v, SIZEOF_VEC4); }

  // binary operators
  Vector4& operator =(const Vector4& other);

  bool operator ==(const Vector4& other) const;

  Vector4& operator +=(const Vector4& other);

  Vector4& operator -=(const Vector4& other);

  Vector4& operator /=(const Vector4& other);

  Vector4& operator *=(const Vector4& other);

  Vector4& operator /=(const F32& other);

  Vector4& operator *=(const F32& factor);

  friend inline Vector4 operator +(Vector4 lhs, const Vector4& rhs) 
  {
    return lhs += rhs;
  }

  friend inline Vector4 operator -(Vector4 lhs, const Vector4& rhs) 
  {
    return lhs -= rhs;
  }

  friend inline Vector4 operator /(Vector4 lhs, const Vector4& rhs) 
  {
    return lhs /= rhs;
  }

  friend inline Vector4 operator *(Vector4 lhs, const Vector4& rhs) 
  {
    return lhs *= rhs;
  }

  Vector4& operator -();

  // access operators
  F32& operator [](const UI32 index);

  const F32& operator [](const UI32 index) const;

  Vector3 point3D() const; // perspective divide

  Vector3 toVector3() const; // drop w

  static Vector4 toHomogeneous(const Vector3& point, F32 w = 1.0f);

  F32 dot(const Vector4& other) const;

  F32 length() const;

  Vector4 normalize() const;

  F32 sum() const;
} Vector4;

// binary operators
Vector4 operator /(Vector4 lhs, const F32& rhs);

Vector4 operator *(Vector4 lhs, const F32& rhs);

Vector4 operator *(const F32& lhs, Vector4 rhs);

// stream operators
std::istream& operator >> (std::istream& inStream, Vector4& vector);

std::ostream& operator << (std::ostream& outStream, const Vector4& vector);

#endif // ! VECTOR4_H