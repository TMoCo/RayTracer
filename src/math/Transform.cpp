//
// Transform class definition
//

#include <math/Transform.h>

// TODO: implement transform methods
Transform::Transform(Vector3 position, Quaternion rotation, Vector3 scale) 
{
  // construct matrix and its inverse
  matrix = Matrix4::scale(scale) * Matrix4::rotation(rotation) * Matrix4::translation(position);
  inverseMatrix = Matrix4::translation(-position)* Matrix4::transpose(Matrix4::rotation(rotation)) 
    * Matrix4::scale(1.0f / scale);
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