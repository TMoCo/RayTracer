//
// Vector3 class definition
//

#include <math/thomath.h>

#include <core/core.h>

#include <string.h>
#include <math.h>
#include <iomanip>

Vector3& Vector3::operator =(const Vector3& other) 
{
  x = other.x;
  y = other.y;
  z = other.z;
  return *this;
}

bool Vector3::operator ==(const Vector3& other) const 
{
  return (x == other.x) && (y == other.y) && (z == other.z);
}

Vector3& Vector3::operator +=(const Vector3& other) 
{
  __v = _mm_add_ps(__v, other.__v);
  return *this;
}

Vector3& Vector3::operator -=(const Vector3& other) 
{
  __v = _mm_sub_ps(__v, other.__v);
  return *this;
}

Vector3& Vector3::operator /=(const Vector3& other) 
{
  __v = _mm_div_ps(__v, other.__v);
  return *this;
}

Vector3& Vector3::operator *=(const Vector3& other) 
{
  __v = _mm_mul_ps(__v, other.__v);
  return *this;
}

Vector3& Vector3::operator /=(const F32& factor) 
{
  __v = _mm_div_ps(__v, _mm_set_ps1(factor));
  return *this;
}

Vector3& Vector3::operator *=(const F32& factor) 
{
  __v = _mm_mul_ps(__v, _mm_set_ps1(factor));
  return *this;
}

Vector3 Vector3::operator -() const 
{
  return *this * -1.0f;
}

F32& Vector3::operator [](const UI32 index) 
{ 
  return _v[index];
}

const F32& Vector3::operator [](const UI32 index) const 
{
  return _v[index];
}

F32 Vector3::dot(const Vector3& other) const 
{
  __m128 dot = _mm_mul_ps(__v, other.__v);
  __m128 shuf = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(2, 3, 0, 1)); // [ C D | A B ]
  dot = _mm_add_ps(dot, shuf); // sums = [ D+C C+D | B+A A+B ]
  shuf = _mm_movehl_ps(shuf, dot); // [ C D | D+C C+D ]
  dot = _mm_add_ss(dot, shuf);
  return _mm_cvtss_f32(dot); // "free" instruction for getting lowest fp value in quadword
}

/*
Vector3 Vector3::Cross(const Vector3& other) const {
    return {
        _v[1] * other[2] - _v[2] * other[1],
        _v[2] * other[0] - _v[0] * other[2], 
        _v[0] * other[1] - _v[1] * other[0]};
}
*/

Vector3 Vector3::cross(const Vector3& other) const 
{
  __m128 res = _mm_sub_ps(
    _mm_mul_ps(
      _mm_shuffle_ps(__v, __v, _MM_SHUFFLE(3, 0, 2, 1)),
      _mm_shuffle_ps(other.__v, other.__v, _MM_SHUFFLE(3, 1, 0, 2))), 
    _mm_mul_ps(
      _mm_shuffle_ps(__v, __v, _MM_SHUFFLE(3, 1, 0, 2)),
      _mm_shuffle_ps(other.__v, other.__v, _MM_SHUFFLE(3, 0, 2, 1))));
  return Vector3{reinterpret_cast<F32*>(&res)};
}

F32 Vector3::length() const 
{ 
#if (__cplusplus >= 201703L)
  return std::hypot(_v[0], _v[1], _v[2]);
#else
  return std::sqrt(dot(*this));
#endif
}

F32 Vector3::lengthSquared() const
{
  F32 length = this->length();
  return length * length;
}

Vector3 Vector3::normalize() const 
{
  return *this / length();
}

Vector3 Vector3::reflect(const Vector3& v, const Vector3& normal) 
{
  return v - 2.0f * v.dot(normal) * normal; // assumes unit normal
}

Vector3 Vector3::refract(const Vector3& v, const Vector3& normal, const F32& iorRatio)
{
  F32 cosTheta = fmin(v.dot(normal), 1.0f); // assumes unit length
  Vector3 perpendicular = iorRatio * (v + cosTheta * normal);
  Vector3 parallel = -sqrtf(fabs(1.0f - perpendicular.lengthSquared())) * normal;
  return perpendicular * parallel;
}

Vector3 operator /(Vector3 lhs, const F32& rhs)
{
  return lhs /= rhs;
}

Vector3 operator /(const F32 lhs, Vector3& rhs)
{
  return Vector3{ lhs } / rhs;
}


Vector3 operator *(const F32& lhs, Vector3 rhs)
{
  return rhs *= lhs;
}

Vector3 operator *(Vector3 lhs, const F32& rhs)
{
  return lhs *= rhs;
}

std::istream& operator >> (std::istream& inStream, Vector3& vector)
{
  return inStream >> vector[0] >> vector[1] >> vector[2];
}

std::ostream& operator << (std::ostream& outStream, const Vector3& vector)
{
  return outStream << vector[0] << ' ' << vector[1] << ' ' << vector[2];
}