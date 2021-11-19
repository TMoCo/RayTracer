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

Ray Transform::transformRay(const Ray& ray)
{
  return { transformPoint(ray.origin), transformVector3(ray.direction)};
}

Vector3 Transform::transformPoint(const Vector3& point)
{
  return (matrix * Vector4::toHomogeneous(point)).toPoint();
}

Vector3 Transform::transformNormal(const Vector3& normal)
{
  return (Matrix4::transpose(inverseMatrix) * Vector4::toHomogeneous(normal)).toVector3();
}

Vector3 Transform::transformVector3(const Vector3& vector3)
{
  return (matrix * Vector4::toHomogeneous(vector3)).toVector3();
}