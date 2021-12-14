/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Transform class definition
//

#include <scene/Transform.h>

Transform::Transform(Vector3 position, Quaternion rotation, Vector3 scale) :
  position(position), rotation(rotation), scale(scale), outOfDate(true)
{
  // updateMatrixRepresentations();
}

Transform Transform::applyToTransform(const Transform& other) const
{
  return { position + Quaternion::rotateVector(other.position, rotation), other.rotation * rotation,
    other.scale * scale };
}

Ray Transform::applyToRay(const Ray& ray) const
{
  return { applyToPoint(ray.origin), applyToVector3(ray.direction)};
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
  return Quaternion::rotateVector(normal, rotation).normalize();
}

Vector3 Transform::applyInverseToNormal(const Vector3& normal) const
{
  return Quaternion::rotateVector(normal, rotation.inverse()).normalize();
}

Vector3 Transform::applyToVector3(const Vector3& vector3) const
{
  return (matrix * Vector4{ vector3 }).toVector3();
}

Vector3 Transform::applyInverseToVector3(const Vector3& vector3) const
{
  return (inverseMatrix * Vector4{ vector3 }).toVector3();
}

Quaternion Transform::applyToRotation(const Quaternion& rotationAsQuat) const
{
  return rotationAsQuat * rotation;
}

Quaternion Transform::applyInverseToRotation(const Quaternion& rotationAsQuat) const
{
  return rotationAsQuat * rotation.inverse();
}


void Transform::updateMatrixRepresentations()
{
  // matrix = Matrix4::translation(position) * Matrix4::rotation(rotation) * Matrix4::scale(scale);
  matrix = Matrix4::rotation(rotation);
  matrix[3][0] = position.x;
  matrix[3][1] = position.y;
  matrix[3][2] = position.z;
  matrix[0][0] *= scale.x;
  matrix[1][1] *= scale.y;
  matrix[2][2] *= scale.z;

  //inverseMatrix = Matrix4::scale(1.0f / scale) * Matrix4::transpose(Matrix4::rotation(rotation))
  //  * Matrix4::translation(-position);
  Quaternion inverseRotation = rotation.inverse();
  Vector3 inverseScale = 1.0f / scale;
  Vector3 inversePosition = Quaternion::rotateVector(-position, inverseRotation) * inverseScale;

  inverseMatrix = Matrix4::rotation(inverseRotation);
  inverseMatrix[0][0] *= inverseScale.x;
  inverseMatrix[1][1] *= inverseScale.y;
  inverseMatrix[2][2] *= inverseScale.z;
  inverseMatrix[3][0] = inversePosition.x;
  inverseMatrix[3][1] = inversePosition.y;
  inverseMatrix[3][2] = inversePosition.z;
}

const Matrix4& Transform::getMatrixRepresentation() const
{ 
  return matrix; 
}

const Matrix4& Transform::getInverseMatrixRepresentation() const 
{
  return inverseMatrix;
}