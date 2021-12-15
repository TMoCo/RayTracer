/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Vector4 class definition
//

#include <math/Vector4.h>

Vector4& Vector4::operator =(const Vector4& other) 
{
  x = other.x;
  y = other.y;
  z = other.z;
  w = other.w;
  return *this;
}

bool Vector4::operator ==(const Vector4& other) const 
{
  return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
}

Vector4& Vector4::operator +=(const Vector4& other) 
{
  __v = _mm_add_ps(__v, other.__v);
  return *this;    
}

Vector4& Vector4::operator -=(const Vector4& other) 
{
  __v = _mm_sub_ps(__v, other.__v);
  return *this;
}

Vector4& Vector4::operator /=(const Vector4& other) 
{
  __v = _mm_div_ps(__v, other.__v);
  return *this;
}

Vector4& Vector4::operator *=(const Vector4& other)
{
  __v = _mm_mul_ps(__v, other.__v);
  return *this;
}

Vector4& Vector4::operator /=(const F32& factor) 
{
  __v = _mm_div_ps(__v, _mm_set_ps1(factor));
  return *this;
}

Vector4& Vector4::operator *=(const F32& factor) 
{
  __v = _mm_mul_ps(__v, _mm_set_ps1(factor));
  return *this;
}

Vector4 Vector4::operator -() const
{
  return { -x, -y, -z, -w };
}

F32& Vector4::operator [](const UI32 index) 
{
  return _v[index];
}

const F32& Vector4::operator [](const UI32 index) const
{
  return _v[index];
}

Vector3 Vector4::toPoint() const 
{
  return w == 1 ? Vector3{ x, y, z } : Vector3{ x, y, z } / w;
}

Vector3 Vector4::toVector3() const 
{
  return { _v };
}

F32 Vector4::dot(const Vector4& other) const
{
  __m128 dot = _mm_mul_ps(__v, other.__v);
  __m128 shuf = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(2, 3, 0, 1)); // [ C D | A B ]
  dot = _mm_add_ps(dot, shuf); // [ D+C C+D | B+A A+B ] (reuse dot)
  shuf = _mm_movehl_ps(shuf, dot); // [ C D | D+C C+D ]
  dot = _mm_add_ss(dot, shuf);
  return _mm_cvtss_f32(dot); // "free" instruction for getting lowest fp value in quadword
}

F32 Vector4::length() const 
{
  return std::sqrt(this->dot(*this));
}

Vector4 Vector4::normalize() const 
{
  return *this / length();
}

F32 Vector4::sum() const 
{
  return _v[0] + _v[1] + _v[2] + _v[3];
}

Vector4 operator /(Vector4 lhs, const F32& rhs)
{
  return lhs /= rhs;
}

Vector4 operator *(Vector4 lhs, const F32& rhs)
{
  return lhs *= rhs;
}

Vector4 operator *(const F32& lhs, Vector4 rhs)
{
  return rhs * lhs;
}

std::istream& operator >> (std::istream& inStream, Vector4& vector)
{
  return inStream >> vector[0] >> vector[1] >> vector[2] >> vector[3];
}

std::ostream& operator << (std::ostream& outStream, const Vector4& vector)
{
  return outStream << vector[0] << ' ' << vector[1] << ' ' << vector[2] << ' ' << vector[3];
}