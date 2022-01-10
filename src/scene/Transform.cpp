/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Transform class definition
//

#include <scene/Transform.h>
#include <render/raytracer/Surfel.h>

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

Surfel Transform::applyToSurfel(const Surfel& surfel) const
{
  Surfel result{};
  result.position = applyToPoint(surfel.position);
  result.normal = applyToNormal(surfel.normal);
  result.outDirection = applyToVector3(surfel.outDirection);
  return result;
}

Surfel Transform::applyInverseToSurfel(const Surfel& surfel) const
{
  Surfel result{};
  result.position = applyInverseToPoint(surfel.position);
  result.normal = applyInverseToNormal(surfel.normal);
  result.outDirection = applyInverseToVector3(surfel.outDirection);
  return result;
}

rt::Ray Transform::applyToRay(const rt::Ray& ray) const
{
  return { applyToPoint(ray.origin), applyToVector3(ray.direction), ray.tMax};
}

rt::Ray Transform::applyInverseToRay(const rt::Ray& ray) const
{
  return { applyInverseToPoint(ray.origin), applyInverseToVector3(ray.direction), ray.tMax };
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
  float x = normal[0], y = normal[1], z = normal[2];
  return Vector3
  {
    matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z,
    matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z,
    matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z
  }.normalize();
 }

Vector3 Transform::applyInverseToNormal(const Vector3& normal) const
{
  float x = normal[0], y = normal[1], z = normal[2];
  return Vector3
  {
    inverseMatrix[0][0] * x + inverseMatrix[0][1] * y + inverseMatrix[0][2] * z,
    inverseMatrix[1][0] * x + inverseMatrix[1][1] * y + inverseMatrix[1][2] * z,
    inverseMatrix[2][0] * x + inverseMatrix[2][1] * y + inverseMatrix[2][2] * z
  }.normalize();
}

Vector3 Transform::applyToVector3(const Vector3& vector) const
{
  float x = vector[0], y = vector[1], z = vector[2];
  return
  {
    matrix[0][0] * x + matrix[1][0] * y + matrix[2][0] * z,
    matrix[0][1] * x + matrix[1][1] * y + matrix[2][1] * z,
    matrix[0][2] * x + matrix[1][2] * y + matrix[2][2] * z
  };
}

Vector3 Transform::applyInverseToVector3(const Vector3& vector) const
{
  float x = vector[0], y = vector[1], z = vector[2];
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

Vector3 Transform::getTranslation() const
{
  return { matrix[3][0], matrix[3][1], matrix[3][2] };
}