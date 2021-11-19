//
// Transform class definition
//

#include <scene/Transform.h>

// TODO: implement transform methods
Transform::Transform(Vector3 position, Quaternion rotation, Vector3 scale) 
{
  // construct matrix and its inverse
  matrix = Matrix4::translation(position) * Matrix4::rotation(rotation) * Matrix4::scale(scale);
  inverseMatrix = Matrix4::scale(1.0f / scale) * Matrix4::transpose(Matrix4::rotation(rotation)) * Matrix4::translation(-position);
}

void Transform::rotate(const Quaternion& quaternion)
{
  matrix *= Matrix4::rotation(quaternion);
}

void Transform::translate(const Vector3& vec3) 
{  
  matrix *= Matrix4::translation(vec3);
}

void Transform::scale(const Vector3& scale)
{
  matrix *= Matrix4::scale(scale);
}