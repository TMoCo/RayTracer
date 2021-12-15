/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Quaternion class definition
//

#include <math/thomath.h>

Quaternion& Quaternion::operator = (const Quaternion &other) 
{
  std::memcpy(&_q[0], &other._q[0], SIZEOF_VEC4);
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
  Vector3 v1 = _q.toVector3();
  Vector3 v2 = other._q.toVector3();
  *this = Quaternion{
    _q[3] * v2 + other._q[3] * v1 + v1.cross(v2),
    _q[3] * other._q[3] - v1.dot(v2)
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
  return _q.toVector3();
}

F32 Quaternion::scalar() const 
{
  return _q[3];
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
  return { -_q[0], -_q[1], -_q[2], _q[3] };
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
  // pitch -> x rotation / yaw -> y rotation / roll -> z rotation
  F32 pitch2 = radians(pitch * 0.5f), yaw2 = radians(yaw * 0.5f), roll2 = radians(roll * 0.5f);
  F32 sinpitch = sinf(pitch2), sinyaw = sinf(yaw2), sinroll = sinf(roll2);
  F32 cospitch = cosf(pitch2), cosyaw = cosf(yaw2), cosroll = cosf(roll2);
  return {
  sinyaw * sinpitch * cosroll + cosyaw * cospitch * sinroll,
  sinyaw * cospitch * cosroll + cosyaw * sinpitch * sinroll,
  cosyaw * sinpitch * cosroll - sinyaw * cospitch * sinroll,
  cosyaw * cospitch * cosroll - sinyaw * sinpitch * sinroll
  };
}

Quaternion Quaternion::getRotationFrom(const Vector3& from, const Vector3& to)
{
  return Quaternion{ from.cross(to), sqrtf(from.dot(from) * to.dot(to) + from.dot(to)) }.unit();
}

Vector3 Quaternion::rotateVector(const Vector3& vector, const Quaternion& quaternion)
{
  Vector3 v = quaternion._q.toVector3();
  Vector3 t = (2.0f * v).cross(vector);
  return vector + quaternion._q[3] * t + v.cross(t);
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
  return inStream >> quaternion._q;
}

std::ostream & operator << (std::ostream &outStream, const Quaternion &quaternion) 
{
  return outStream << quaternion._q; 
}
