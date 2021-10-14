//
// Vector2 class definition
//

#include <core/core.h>

#include <math/Vector2.h>

#include <cstring>


Vector2& Vector2::operator =(const Vector2& other) 
{
  std::memcpy(this->_v, other._v, SIZEOF_VEC2); // 2 * sizeof(F32) = 8
  return *this;
}

Vector2& Vector2::operator -=(const Vector2& other) 
{
  this->x -= other.x;
  this->y -= other.y;
  return *this;
}

Vector2& Vector2::operator +=(const Vector2& other) 
{
  this->x += other.x;
  this->y += other.y;
  return *this;
}

Vector2& Vector2::operator *=(const Vector2& other)
{
  this->x *= other.x;
  this->y *= other.y;
  return *this;
}

Vector2& Vector2::operator /=(const F32& factor) 
{
  F32 inv = 1.0f / factor;
  this->x *= inv;
  this->y *= inv;
  return *this;
}

F32& Vector2::operator [](const UI32 index) 
{ 
  return this->_v[index]; // index greater than 1 not defined
}

const F32& Vector2::operator [](const UI32 index) const 
{
  return this->_v[index];
}

Vector2 operator *(Vector2 lhs, const F32& rhs) 
{
  return Vector2{lhs[0] * rhs, lhs[1] * rhs};
}

Vector2 operator /(Vector2 lhs, const F32& rhs) 
{
  return lhs /= rhs;
}

Vector2 operator *(const F32& lhs, Vector2 rhs) 
{
  return Vector2{rhs[0] * lhs, rhs[1] * lhs};
}