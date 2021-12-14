/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Transform class declaration
//

#ifndef TRANSFORM_H_
#define TRANSFORM_H_ 1

#include <math/thomath.h>

#include <render/raytracer/Ray.h>

class Transform {
  friend class Node;

public:
  Transform(Vector3 position = {}, Quaternion rotation = {}, Vector3 scale = { 1.0f, 1.0f, 1.0f });

  Transform applyToTransform(const Transform& other) const;

  Ray applyToRay(const Ray& ray) const;

  Ray applyInverseToRay(const Ray& ray) const;

  Vector3 applyToPoint(const Vector3& point) const;

  Vector3 applyInverseToPoint(const Vector3& point) const;

  Vector3 applyToNormal(const Vector3& normal) const;

  Vector3 applyInverseToNormal(const Vector3& normal) const;

  Vector3 applyToVector3(const Vector3& vector3) const;

  Vector3 applyInverseToVector3(const Vector3& vector3) const;

  Quaternion applyToRotation(const Quaternion& rotationAsQuat) const;
  
  Quaternion applyInverseToRotation(const Quaternion& rotationAsQuat) const;

  void updateMatrixRepresentations();

  const Matrix4& getMatrixRepresentation() const;

  const Matrix4& getInverseMatrixRepresentation() const;

//protected:
  Vector3 position;

  Quaternion rotation;

  Vector3 scale;

  Matrix4 matrix;

  Matrix4 inverseMatrix;

  bool outOfDate;
};

#endif // !TRANSFORM_H_
