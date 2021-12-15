/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Transform class definition
//

#include <scene/Transform.h>

Transform::Transform()
  : matrix{ Matrix4::identity() },
  inverseMatrix{ Matrix4::identity() },
  outOfDate{ false }
{ }

Transform::Transform(Vector3 translation, Quaternion rotation, Vector3 scale)
  : matrix{ Matrix4::scale(scale) * Matrix4::rotation(rotation) * Matrix4::translation(translation) },
  inverseMatrix{ Matrix4::translation(-translation) * Matrix4::transpose(Matrix4::rotation(rotation)) * Matrix4::scale(1.0f / scale) },
  outOfDate{ false }
{ }

void Transform::translateBy(const Vector3& translation)
{
  matrix = matrix * Matrix4::translation(translation);
  inverseMatrix = Matrix4::translation(-translation) * inverseMatrix;
}

void Transform::rotateBy(const Quaternion& rotation)
{
  Matrix4 rotationMatrix = Matrix4::rotation(rotation);
  matrix = matrix * rotationMatrix;
  inverseMatrix = Matrix4::transpose(rotationMatrix) * inverseMatrix;
}

void Transform::scaleBy(const Vector3& scale)
{
  matrix = matrix * Matrix4::scale(scale);
  inverseMatrix = Matrix4::scale(1.0f / scale) * inverseMatrix;
}

Transform Transform::applyToTransform(const Transform& other) const
{
  Transform result;
  result.matrix = matrix * other.matrix;
  result.inverseMatrix = other.inverseMatrix * inverseMatrix;
  return result;
}

Ray Transform::applyToRay(const Ray& ray) const
{
  return { applyToPoint(ray.origin), applyToVector3(ray.direction) };
}

Ray Transform::applyInverseToRay(const Ray& ray) const
{
  return { applyInverseToPoint(ray.origin), applyInverseToVector3(ray.direction) };
}

Vector3 Transform::applyToPoint(const Vector3& point) const
{
  return (matrix * Vector4{ point }).toPoint();
}

Vector3 Transform::applyInverseToPoint(const Vector3& point) const
{
  return (inverseMatrix * Vector4{ point }).toPoint();
}

Vector3 Transform::applyToNormal(const Vector3& normal) const
{
  return (Matrix4::transpose(matrix) * Vector4 { normal }).toVector3().normalize();
}

Vector3 Transform::applyInverseToNormal(const Vector3& normal) const
{
  return (matrix * Vector4 { normal }).toVector3().normalize();
}

Vector3 Transform::applyToVector3(const Vector3& vector) const
{
  F32 x = vector.x, y = vector.y, z = vector.z;
  return
  {
    matrix[0][0] * x + matrix[1][0] * y + matrix[2][0] * z,
    matrix[0][1] * x + matrix[1][1] * y + matrix[2][1] * z,
    matrix[0][2] * x + matrix[1][2] * y + matrix[2][2] * z
  };
}

Vector3 Transform::applyInverseToVector3(const Vector3& vector) const
{
  F32 x = vector.x, y = vector.y, z = vector.z;
  return
  {
    inverseMatrix[0][0] * x + inverseMatrix[1][0] * y + inverseMatrix[2][0] * z,
    inverseMatrix[0][1] * x + inverseMatrix[1][1] * y + inverseMatrix[2][1] * z,
    inverseMatrix[0][2] * x + inverseMatrix[1][2] * y + inverseMatrix[2][2] * z
  };
}

const Matrix4& Transform::getMatrix() const
{ 
  return matrix; 
}

const Matrix4& Transform::getInverseMatrix() const 
{
  return inverseMatrix;
}