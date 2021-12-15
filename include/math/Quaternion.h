/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Quaternionernion class declaration
//

#ifndef QUATERNION_H_
#define QUATERNION_H_ 1

#include <core/types.h>

#include <math/Vector3.h>
#include <math/Vector4.h>

typedef struct Quaternion {
public:
  Vector4 _q;

  Quaternion(const Vector3& vector = {}, F32 scalar = 1.0f) : _q{ vector.x, vector.y, vector.z, scalar } {}

  Quaternion(const Quaternion& other) : _q{ other._q } {}

  Quaternion(F32 x, F32 y, F32 z, F32 w) : _q{ x, y, z, w } {}

  Quaternion(const F32* values) : _q{ values } {}

  Quaternion(const Vector4& vector) : _q{ vector } {}

  Quaternion&operator = (const Quaternion &other);

  bool operator ==(const Quaternion& other) const;

  Quaternion& operator +=(const Quaternion& other);

  Quaternion& operator -=(const Quaternion& other);

  Quaternion& operator /=(const Quaternion& other);

  Quaternion& operator *=(const Quaternion& other);

  Quaternion& operator /=(const F32& other);

  Quaternion& operator *=(const F32& factor);

  friend inline Quaternion operator +(Quaternion lhs, const Quaternion& rhs) 
  {
    return lhs += rhs;
  }

  friend inline Quaternion operator -(Quaternion lhs, const Quaternion& rhs) 
  {
    return lhs -= rhs;
  }

  friend inline Quaternion operator /(Quaternion lhs, const Quaternion& rhs) 
  {
    return lhs /= rhs;
  }

  friend inline Quaternion operator *(Quaternion lhs, const Quaternion& rhs) 
  {
    return lhs *= rhs;
  }
  
  Quaternion& operator -();

  F32& operator [](const UI32 index);

  const F32& operator [](const UI32 index) const;

  Vector3 vector() const;

  F32 scalar() const;

  F32 norm() const;

  Quaternion unit() const;

  Quaternion conjugate() const;

  Quaternion inverse() const;

  static Quaternion angleAxis(F32 angle, const Vector3& axis);

  static Quaternion eulerAngles(F32 pitch, F32 yaw, F32 roll);

  static Quaternion getRotationFrom(const Vector3& from, const Vector3& to);

  static Vector3 rotateVector(const Vector3& vector, const Quaternion& quaternion);

} Quaternion;


Quaternion operator /(Quaternion lhs, const F32 rhs);

Quaternion operator *(Quaternion lhs, const F32 rhs);

Quaternion operator *(const F32 lhs, Quaternion& rhs);

std::istream & operator >> (std::istream &inStream, Quaternion& value);

std::ostream & operator << (std::ostream &outStream, const Quaternion& value);

#endif // ! QUATERNION_H_
