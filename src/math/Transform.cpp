//
// Transform class definition
//

#include <math/Transform.h>

Matrix4 Transform::toWorldMatrix() const 
{
  return Matrix4::rotationMatrix(orientation) * Matrix4::translationMatrix(position);
}

Vector3 Transform::transformPoint(const Vector3& vec3) const 
{
    return (toWorldMatrix() * Vector4::toHomogeneous(vec3))._v;
}

Vector3 Transform::rotate(const Vector3& vec3) const 
{
  return Quaternion::rotateVector(vec3, orientation);
    // return (Matrix4::RotationMatrix(orientation) * Vector4{vec3})._v;
}

Vector3 Transform::translate(const Vector3& vec3) const 
{
  return position + vec3;
}

Transform Transform::transformChild(const Transform& parent, const Transform& child)
{
  Transform composite;
  // multiply parent by child
  composite.orientation = parent.orientation * child.orientation;
  // rotate the child translation by the parent orientation and translate parent
  composite.position = parent.position + parent.rotate(child.position);
  return composite;
}
