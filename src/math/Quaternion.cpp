/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Quaternion class definition
//

#include <math/thomath.h>

// assignment operator
Quaternion& Quaternion::operator = (const Quaternion &other) 
{
  std::memcpy(_q._v, other._q._v, SIZEOF_VEC4);
  return *this;
}

bool Quaternion::operator ==(const Quaternion& other) const 
{
  return _q == other._q;
}

Quaternion& Quaternion::operator +=(const Quaternion& other)
{
  _q += other._q;
  return *this;
}

Quaternion& Quaternion::operator -=(const Quaternion& other) 
{
  _q -= other._q;
  return *this;
}

Quaternion& Quaternion::operator /=(const Quaternion& other) 
{
  _q *= other.inverse()._q;
  return *this;
}

Quaternion& Quaternion::operator *=(const Quaternion& other) 
{
  Vector3 v1 = { _v };
  Vector3 v2 = { other._v };
  *this = Quaternion{
    _scalar * v2 + other._scalar * v1 + v1.cross(v2),
    _scalar * other._scalar - v1.dot(v2)
  };
  return *this;
}

Quaternion& Quaternion::operator /=(const F32& factor) 
{
  _q /= factor;
  return *this;
}

Quaternion& Quaternion::operator *=(const F32& factor)
{
  _q *= factor;
  return *this;
}

Quaternion& Quaternion::operator -() 
{
  return *this *= -1.0f;
}

F32& Quaternion::operator [](const UI32 index) 
{
  return _q[index];
}

const F32& Quaternion::operator [](const UI32 index) const 
{
  return _q[index];
}

Vector3 Quaternion::vector() const 
{
  return Vector3{ _v };
}

F32 Quaternion::scalar() const 
{
  return _scalar;
}

float Quaternion::norm() const 
{
  return _q.length();
}

Quaternion Quaternion::unit() const 
{
  return *this / norm();
}

Quaternion Quaternion::conjugate() const 
{
  return { -Vector3{ _v }, _scalar };
}

Quaternion Quaternion::inverse() const 
{
  return conjugate() / _q.dot(_q);
}

Quaternion Quaternion::angleAxis(F32 angle, const Vector3& axis)
{
  return Quaternion(axis.normalize() * sinf(angle * 0.5f), cosf(angle * 0.5f));
}

Quaternion Quaternion::eulerAngles(F32 pitch, F32 yaw, F32 roll)
{
  Quaternion quaternion{};
  F32 roll2 = radians(roll * 0.5f), pitch2 = radians(pitch * 0.5f), yaw2 = radians(yaw * 0.5f);
  F32 sp = sinf(pitch2), cp = cosf(pitch2);
  F32 sy = sinf(yaw2), cy = cosf(yaw2);
  F32 sr = sinf(roll2), cr = cosf(roll2);
  quaternion._v[0] = sp * cy * cr + cp * sy * sr;
  quaternion._v[1] = cp * sy * cr - sp * cy * sr;
  quaternion._v[2] = sp * sy * cr + cp * cy * sr;
  quaternion._scalar   = cr * cp * cy - sr * sp * sy;
  return quaternion;
}

Quaternion Quaternion::getRotationFrom(const Vector3& from, const Vector3& to)
{
  return Quaternion{ from.cross(to), sqrtf(from.dot(from) * to.dot(to) + from.dot(to)) }.unit();
}

Vector3 Quaternion::rotateVector(const Vector3& vector, const Quaternion& quaternion)
{
  Vector3 t = (2.0f * Vector3{ quaternion._v }).cross(vector);
  return vector + quaternion._scalar * t + Vector3{ quaternion._v }.cross(t);
}

Quaternion operator /(Quaternion lhs, const F32 rhs) 
{
  return lhs /= rhs;
}

Quaternion operator *(const F32 lhs, Quaternion& rhs) 
{
  return rhs *= lhs;
}

Quaternion operator *(Quaternion lhs, const F32 rhs) 
{
  return rhs * lhs;
}

std::istream& operator >> (std::istream &inStream, Quaternion &quaternion) 
{
  return inStream >> quaternion._q; // Vector4
}

std::ostream & operator << (std::ostream &outStream, const Quaternion &quaternion) 
{
  return outStream << quaternion._q; 
}
