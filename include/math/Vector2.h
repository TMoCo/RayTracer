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

constexpr size_t SIZE_OF_VEC2 = 0x8;

typedef class Vector2 {
public:
  Vector2() : _v{} {}
  
  Vector2(F32 x, F32 y) : x(x), y(y) {}
  
  Vector2(const Vector2& other) : _v{ other.x, other.y } {}

  Vector2(const F32* values) : _v{ values[0], values[1] } {}

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

private:
  union
  {
    F32 _v[2];

    struct
    {
      F32 x, y;
    };
  };

} Vector2;

Vector2 operator *(Vector2 lhs, const F32& rhs);

Vector2 operator /(Vector2 lhs, const F32& rhs);

Vector2 operator *(const F32& lhs, Vector2 rhs);



#endif // ! VECTOR2_H
