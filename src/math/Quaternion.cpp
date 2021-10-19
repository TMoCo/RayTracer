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
  *this = Quaternion{
    _scalar * other._vector + other._scalar * _vector + _vector.cross(other._vector),
    _scalar * other._scalar - _vector.dot(other._vector)
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
  return _vector;
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
  return { -_vector, _scalar };
}

Quaternion Quaternion::inverse() const 
{
  return conjugate() / _q.dot(_q);
}

Quaternion Quaternion::angleAxis(F32 angle, const Vector3& axis)
{
  return Quaternion(axis.normalize() * sinf(angle * 0.5f), cosf(angle * 0.5f));
}

Quaternion Quaternion::getRotationFrom(const Vector3& from, const Vector3& to)
{
  return Quaternion{ from.cross(to), sqrtf(from.dot(from) * to.dot(to) + from.dot(to)) }.unit();
}

Vector3 Quaternion::rotateVector(const Vector3& vector, const Quaternion& quaternion)
{
  Vector3 t = (2.0f * quaternion._vector).cross(vector);
  return vector + quaternion._scalar * t + quaternion._vector.cross(t);
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
