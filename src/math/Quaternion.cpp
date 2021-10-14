//
// Quaternion class definition
//

#include <core/core.h>

#include <math/Quaternion.h>

#include <cmath>
#include <cstring>

Quaternion::Quaternion(const Quaternion& other) 
  : _q{other._q} 
{

}

Quaternion::Quaternion(const Vector3& v, F32 s) 
  : _vector{v}, 
    _scalar(s)
{

}

Quaternion::Quaternion(F32 x, F32 y, F32 z, F32 w) 
  : _q{x, y, z, w}
{
  // avoid
} 

Quaternion::Quaternion(const F32* values) 
  : _q{values}
{

}

Quaternion::Quaternion(const Vector4& vector) 
  : _q{vector} 
{

}

// assignment operator
Quaternion& Quaternion::operator = (const Quaternion &other) 
{
  std::memcpy(this->_q._v, other._q._v, SIZEOF_VEC4);
  return *this;
}

bool Quaternion::operator ==(const Quaternion& other) const 
{
  return this->_q == other._q;
}

Quaternion& Quaternion::operator +=(const Quaternion& other)
{
  this->_q += other._q;
  return *this;
}

Quaternion& Quaternion::operator -=(const Quaternion& other) 
{
  this->_q -= other._q;
  return *this;
}

Quaternion& Quaternion::operator /=(const Quaternion& other) 
{
  this->_q *= other.inverse()._q;
  return *this;
}

Quaternion& Quaternion::operator *=(const Quaternion& other) 
{
  /*
  Vector3 v1{&_q[0]}, v2{&other._q[0]};
  float s1 = _q[3], s2 = other._q[3];
  std::cout << "\nnew rotation\n";
  std::cout << "scalars: " << s1 << " " << s2 << "\n" << _scalar << " " << other._scalar << "\n";
  std::cout << "vectors: " << v1 << " " << v2 << "\n" << _vector << " " << other._vector << "\n";
  std::cout << _scalar * other._scalar << " " <<  _vector.Dot(other._vector) << "\n";

  Quaternion q = Quaternion{s1 * v2 + s2 * v1 + v1.Cross(v2), s1 * s2 - v1.Dot(v2)}; 
  Quaternion p = Quaternion{
        _scalar * other._vector 
      + other._scalar * _vector 
      + _vector.Cross(other._vector), 
      _scalar * other._scalar - _vector.Dot(other._vector)
  };

  std::cout << q << "\n" << p << "\n\n";

  *this = p;
  */
  *this = Quaternion{
    this->_scalar * other._vector + other._scalar * this->_vector + this->_vector.cross(other._vector),
    this->_scalar * other._scalar - this->_vector.dot(other._vector)
  };
  return *this;
}

Quaternion& Quaternion::operator /=(const float& factor) 
{
  this->_q /= factor;
  return *this;
}

Quaternion& Quaternion::operator *=(const float& factor) 
{
  this->_q *= factor;
  return *this;
}

Quaternion& Quaternion::operator -() 
{
  return *this *= -1.0f;
}

F32& Quaternion::operator [](const UI32 index) 
{
  return this->_q[index];
}

const F32& Quaternion::operator [](const UI32 index) const 
{
  return this->_q[index];
}

Vector3 Quaternion::vector() const 
{
  return this->_vector;
}

F32 Quaternion::scalar() const 
{
  return this->_scalar;
}

float Quaternion::norm() const 
{
  return this->_q.length();
}

Quaternion Quaternion::unit() const 
{
  return *this / this->norm();
}

Quaternion Quaternion::conjugate() const 
{
  return {-this->_vector, this->_scalar};
}

Quaternion Quaternion::inverse() const 
{
  return this->conjugate() / this->_q.dot(this->_q);
}

Quaternion Quaternion::angleAxis(const Vector3& axis, F32 angle)
{
  return Quaternion(axis.normalize() * std::sin(angle * 0.5f), std::cos(angle * 0.5f));
}

Quaternion Quaternion::rotation(const Vector3& from, const Vector3& to)
{
  return { from.cross(to), std::sqrt(from.dot(from) * to.dot(to) + from.dot(to)) };
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
