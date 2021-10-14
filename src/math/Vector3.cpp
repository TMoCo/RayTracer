//
// Vec3 class definition
//

#include <math/Vector3.h>

#include <core/core.h>

#include <string.h>
#include <math.h>
#include <iomanip>

Vec3& Vec3::operator =(const Vec3& other) 
{
  memcpy(_v, other._v, SIZEOF_VEC3);
  return *this;
}

bool Vec3::operator ==(const Vec3& other) const 
{
  return (x == other.x) && (y == other.y) && (z == other.z);
}

Vec3& Vec3::operator +=(const Vec3& other) 
{
  __v = _mm_add_ps(__v, other.__v);
  return *this;
}

Vec3& Vec3::operator -=(const Vec3& other) 
{
  __v = _mm_sub_ps(__v, other.__v);
  return *this;
}

Vec3& Vec3::operator /=(const Vec3& other) 
{
  __v = _mm_div_ps(__v, other.__v);
  return *this;
}

Vec3& Vec3::operator *=(const Vec3& other) 
{
  __v = _mm_mul_ps(__v, other.__v);
  return *this;
}

Vec3& Vec3::operator /=(const F32& factor) 
{
  __v = _mm_div_ps(__v, _mm_set_ps1(factor));
  return *this;
}

Vec3& Vec3::operator *=(const F32& factor) 
{
  __v = _mm_mul_ps(__v, _mm_set_ps1(factor));
  return *this;
}

Vec3 Vec3::operator -() const 
{
  return *this * -1.0f;
}

F32& Vec3::operator [](const UI32 index) 
{ 
  return _v[index];
}

const F32& Vec3::operator [](const UI32 index) const 
{
  return _v[index];
}

F32 Vec3::dot(const Vec3& other) const 
{
  __m128 shuf = _mm_shuffle_ps(__v, other.__v, _MM_SHUFFLE(2, 3, 0, 1)); // [ C D | A B ]
  __m128 sums = _mm_add_ps(__v, shuf); // sums = [ D+C C+D | B+A A+B ]
  shuf = _mm_movehl_ps(shuf, sums); // [ C D | D+C C+D ]
  sums = _mm_add_ss(sums, shuf);
  return _mm_cvtss_f32(sums); // "free" instruction for getting lowest fp value in quadword
}

/*
Vec3 Vec3::Cross(const Vec3& other) const {
    return {
        _v[1] * other[2] - _v[2] * other[1],
        _v[2] * other[0] - _v[0] * other[2], 
        _v[0] * other[1] - _v[1] * other[0]};
}
*/

Vec3 Vec3::cross(const Vec3& other) const 
{
  __m128 res = _mm_sub_ps(
    _mm_mul_ps(
      _mm_shuffle_ps(__v, __v, _MM_SHUFFLE(3, 0, 2, 1)),
      _mm_shuffle_ps(other.__v, other.__v, _MM_SHUFFLE(3, 1, 0, 2))), 
    _mm_mul_ps(
      _mm_shuffle_ps(__v, __v, _MM_SHUFFLE(3, 1, 0, 2)),
      _mm_shuffle_ps(other.__v, other.__v, _MM_SHUFFLE(3, 0, 2, 1))));
  return Vec3{reinterpret_cast<F32*>(&res)};
}

F32 Vec3::length() const 
{ 
#if (__cplusplus >= 201703L)
  return std::hypot(_v[0], _v[1], _v[2]);
#else
  return std::sqrt(this->dot(*this));
#endif
}

F32 Vec3::lengthSquared() const
{
  F32 length = this->length();
  return length * length;
}

Vec3 Vec3::normalize() const 
{
  return *this / length();
}

Vec3 Vec3::reflect(const Vec3& v, const Vec3& normal) 
{
  return v - 2.0f * v.dot(normal) * normal; // assumes unit normal
}

Vec3 Vec3::refract(const Vec3& v, const Vec3& normal, const float& iorRatio)
{
  F32 cosTheta = fmin(v.dot(normal), 1.0f); // assumes unit length
  Vec3 perpendicular = iorRatio * (v + cosTheta * normal);
  Vec3 parallel = -sqrtf(fabs(1.0f - perpendicular.lengthSquared())) * normal;
  return perpendicular * parallel;
}

Vector3 operator /(Vector3 lhs, const F32& rhs)
{
  return lhs /= rhs;
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