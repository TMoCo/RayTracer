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

typedef struct Vec4 {
public:
  union {
    F32     _v[4];
    __m128 __v;
    struct {
      F32 x, y, z, w;
    };
  };

  constexpr Vec4() :_v{} {}

  constexpr Vec4(F32 X, F32 Y, F32 Z, F32 W = 1.0f) : _v{ X, Y, Z, W } {}

  Vec4(const F32* values) : _v{} { memcpy(_v, values, SIZEOF_VEC4); }

  Vec4(const Vec4& other) : _v{} {memcpy(_v, other._v, SIZEOF_VEC4); }

  // binary operators
  Vec4& operator =(const Vec4& other);

  bool operator ==(const Vec4& other) const;

  Vec4& operator +=(const Vec4& other);

  Vec4& operator -=(const Vec4& other);

  Vec4& operator /=(const Vec4& other);

  Vec4& operator *=(const Vec4& other);

  Vec4& operator /=(const F32& other);

  Vec4& operator *=(const F32& factor);

  friend inline Vec4 operator +(Vec4 lhs, const Vec4& rhs) 
  {
    return lhs += rhs;
  }

  friend inline Vec4 operator -(Vec4 lhs, const Vec4& rhs) 
  {
    return lhs -= rhs;
  }

  friend inline Vec4 operator /(Vec4 lhs, const Vec4& rhs) 
  {
    return lhs /= rhs;
  }

  friend inline Vec4 operator *(Vec4 lhs, const Vec4& rhs) 
  {
    return lhs *= rhs;
  }

  Vec4& operator -();

  // access operators
  F32& operator [](const UI32 index);

  const F32& operator [](const UI32 index) const;

  Vector3 point3D() const; // perspective divide

  Vector3 toVector3() const; // drop w

  static Vec4 toHomogeneous(const Vector3& point, F32 w = 1.0f);

  F32 dot(const Vec4& other) const;

  F32 length() const;

  Vec4 normalize() const;

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