//
// Vector4 class definition
//

#include <core/core.h>

#include <math/Vector4.h>

#include <cmath>
#include <cstring>
#include <iomanip>

Vector4::Vector4() 
  :_v{} 
{

}

Vector4::Vector4(F32 x, F32 y, F32 z, F32 w) 
  : _v{x, y, z, w} 
{

}

Vector4::Vector4(const F32* v) 
  : _v{} 
{
  std::memcpy(_v, v, SIZEOF_VEC4);
}

Vector4::Vector4(const Vector3& other, F32 W)
  : _v{} 
{
  std::memcpy(_v, other._v, SIZEOF_VEC3);
  w = W;
}

Vector4::Vector4(const Vector4& other) 
  : _v{} 
{
  std::memcpy(_v, other._v, SIZEOF_VEC4);
}
Vector4& Vector4::operator =(const Vector4& other) 
{
  std::memcpy(_v, other._v, SIZEOF_VEC4); 
  return *this;
}

bool Vector4::operator ==(const Vector4& other) const 
{
  return (_v[0] == other._v[0]) && (_v[1] == other._v[1]) && (_v[2] == other._v[2]) && (_v[3] == other._v[3]);
}

Vector4& Vector4::operator +=(const Vector4& other) 
{
  _v4 = _mm_add_ps(_v4, other._v4);
  return *this;    
}

Vector4& Vector4::operator -=(const Vector4& other) 
{
  _v4 = _mm_sub_ps(_v4, other._v4);
  return *this;
}

Vector4& Vector4::operator /=(const Vector4& other) 
{
  _v4 = _mm_div_ps(_v4, other._v4);
  return *this;
}

Vector4& Vector4::operator *=(const Vector4& other)
{
  _v4 = _mm_mul_ps(_v4, other._v4);
  return *this;
}

Vector4& Vector4::operator /=(const F32& factor) 
{
  _v4 = _mm_div_ps(_v4, _mm_load1_ps(&factor));
  return *this;
}

Vector4& Vector4::operator *=(const F32& factor) 
{
  _v4 = _mm_mul_ps(_v4, _mm_load1_ps(&factor));
  return *this;
}

Vector4& Vector4::operator -() 
{
  return *this *= -1.0f;
}

F32& Vector4::operator [](const uint32_t index) 
{
  return _v[index];
}

const F32& Vector4::operator [](const uint32_t index) const 
{
  return _v[index];
}

Vector3 Vector4::Point3D() const 
{
  __m128 vec = _mm_div_ps(_v4, _mm_load1_ps(_v + 3));
  return { (float*)&vec };
}

Vector3 Vector4::ToVector3() const 
{
  return {_v};
}

F32 Vector4::Dot(const Vector4& other) const
{
  __m128 dot = _mm_mul_ps(_v4, other._v4);
  __m128 shuf = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(2, 3, 0, 1)); // [ C D | A B ]
  dot = _mm_add_ps(dot, shuf); // [ D+C C+D | B+A A+B ] (reuse dot)
  shuf = _mm_movehl_ps(shuf, dot); // [ C D | D+C C+D ]
  dot = _mm_add_ss(dot, shuf);
  return _mm_cvtss_f32(dot); // "free" instruction for getting lowest fp value in quadword
}

F32 Vector4::Length() const 
{
  return std::sqrt(Dot(*this));
}

Vector4 Vector4::Normalize() const 
{
  return *this / Length();
}

F32 Vector4::Sum() const 
{
    return _v[0] + _v[1] + _v[2] + _v[3];
}

Vector4 operator /(Vector4 lhs, const F32 rhs) 
{
    return lhs /= rhs;
}

Vector4 operator *(Vector4 rhs, const F32 lhs) 
{
    return rhs *= lhs;
}

Vector4 operator *(const F32 lhs, Vector4& rhs) 
{
    return rhs * lhs;
}


std::istream& operator >> (std::istream &inStream, Vector4 &vector) 
{
    return inStream >> vector[0] >> vector[1] >> vector[2] >> vector[3];
}

std::ostream & operator << (std::ostream &outStream, const Vector4 &vector) {
    return outStream << std::setprecision(4) << vector[0] << ' ' 
    << std::setprecision(4) << vector[1] << ' ' 
    << std::setprecision(4) << vector[2] << ' ' 
    << std::setprecision(4) << vector[3];
}