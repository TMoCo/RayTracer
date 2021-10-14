//
// Transform class definition
//

#include <math/Transform.h>

Transform::Transform(const Vector3& p, const Quaternion& o) 
  : translation{p},
    orientation{o} 
{

}

Matrix4 Transform::toMatrix() const 
{
  return Matrix4::RotationMatrix(orientation) * Matrix4::TranslationMatrix(translation);
}

void Transform::rotate(const Quaternion& r) 
{
  orientation *= r;
}

void Transform::translate(const Vector3& v) 
{
  translation += v;
}

Vector3 Transform::transformPoint(const Vector3& vec3) const 
{
    return (toMatrix() * Vector4::toHomogeneous(vec3))._v;
}

Vector3 Transform::rotatePoint(const Vector3& vec3) const 
{
  return Quaternion::rotateVector(vec3, orientation);
    // return (Matrix4::RotationMatrix(orientation) * Vector4{vec3})._v;
}

Vector3 Transform::translatePoint(const Vector3& vec3) const 
{
  return translation + vec3;
}
