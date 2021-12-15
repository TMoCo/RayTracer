/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// Vector3 class declaration
//

#ifndef VECTOR3_H
#define VECTOR3_H 1

#include <core/types.h>

#include <iostream>
#include <xmmintrin.h>

constexpr UI32 SIZEOF_VEC3 = 0xcU;

typedef class Vector3 {
public:
  constexpr Vector3() : _v{} {}

  constexpr Vector3(F32 value) : _v{ value, value, value } {}

  constexpr Vector3(F32 X, F32 Y, F32 Z) : _v{ X, Y, Z } {}

  constexpr Vector3(const F32* values) : _v{ values[0], values[1], values[2] } {}

  constexpr Vector3(const Vector3& other) : _v{ other.x, other.y, other.z } {}

  Vector3& operator =(const Vector3& other);

  bool operator ==(const Vector3& other) const;

  Vector3& operator +=(const Vector3& other);

  Vector3& operator -=(const Vector3& other);

  Vector3& operator /=(const Vector3& other);

  Vector3& operator *=(const Vector3& other);

  Vector3& operator /=(const F32& other);

  Vector3& operator *=(const F32& factor);

  friend inline Vector3 operator +(Vector3 lhs, const Vector3& rhs) 
  {
    return lhs += rhs;
  }

  friend inline Vector3 operator -(Vector3 lhs, const Vector3& rhs) 
  {
    return lhs -= rhs;
  }

  friend inline Vector3 operator /(Vector3 lhs, const Vector3& rhs) 
  {
    return lhs /= rhs;
  }

  friend inline Vector3 operator *(Vector3 lhs, const Vector3& rhs) 
  {
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

private:
  union {
    F32 _v[3];
    struct {
      F32 x, y, z; 
    };
  };

} Vector3;


Vector3 operator /(Vector3 lhs, const F32& rhs);

Vector3 operator /(const F32 lhs, Vector3& rhs);

Vector3 operator *(const F32& lhs, Vector3 rhs);

Vector3 operator *(Vector3 lhs, const F32& rhs);

std::istream& operator >> (std::istream& inStream, Vector3& vector);

std::ostream& operator << (std::ostream& outStream, const Vector3& vector);

#endif // ! VECTOR3_H
