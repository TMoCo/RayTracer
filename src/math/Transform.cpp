//
// Transform class definition
//

#include <math/Transform.h>

// TODO: implement transform methods
Matrix4 Transform::getMatrix()
{
  return {};
}

void Transform::rotate(const Quaternion& quaternion)
{
  local *= Matrix4::rotation(quaternion);
}

void Transform::translate(const Vector3& vec3) 
{  
  local *= Matrix4::translation(vec3);
}

void Transform::scale(const Vector3& scale)
{
  local *= Matrix4::scale(scale);
}

void Transform::lookAt(Transform& target, Vector3 up)
{
  local *= Matrix4::lookAt(position, target.position, up);
}

void Transform::update()
{
  // update transform 
}