/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// Vector2 class declaration
//

#ifndef VECTOR2_H
#define VECTOR2_H 1

#include <core/types.h>

#include <cstring>

constexpr UI32 SIZEOF_VEC2 = 0x8;

typedef struct Vector2 {
  union { 
    F32 _v[2]; 

    struct {
      F32 x, y;
    };
  };

  Vector2() : _v{} {}
  
  Vector2(F32 X, F32 Y) : x(X), y(Y) {}
  
  Vector2(const Vector2& other) : _v{ other.x, other.y } {}

  Vector2& operator =(const Vector2& other);

  Vector2& operator -=(const Vector2& other);

  Vector2& operator +=(const Vector2& other);

  Vector2& operator *=(const Vector2& other);
  
  Vector2& operator /=(const F32& factor);

  friend inline Vector2 operator -(Vector2 lhs, const Vector2& rhs) 
  {
    return lhs -= rhs;
  }

  friend inline Vector2 operator +(Vector2 lhs, const Vector2& rhs) 
  {
    return lhs += rhs;
  }

  // access operators
  F32& operator [](const UI32 index);

  const F32& operator [](const UI32 index) const;

} Vector2;

Vector2 operator *(Vector2 lhs, const F32& rhs);

Vector2 operator /(Vector2 lhs, const F32& rhs);

Vector2 operator *(const F32& lhs, Vector2 rhs);



#endif // ! VECTOR2_H
