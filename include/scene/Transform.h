/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Transform class declaration
//

#ifndef TRANSFORM_H
#define TRANSFORM_H 1

#include <math/thomath.h>

#include <render/raytracer/Ray.h>

class Surfel;

class Transform {
  friend class Node;

public:
  Transform();
  Transform(Vector3 position, Quaternion rotation, Vector3 scale);

  void translateBy(const Vector3& translation);

  void rotateBy(const Quaternion& rotation);

  void scaleBy(const Vector3& scale);

  Transform applyToTransform(const Transform& other) const;

  Surfel applyToSurfel(const Surfel& surfel) const;
  
  Surfel applyInverseToSurfel(const Surfel& surfel) const;

  Ray applyToRay(const Ray& ray) const;

  Ray applyInverseToRay(const Ray& ray) const;

  Vector3 applyToPoint(const Vector3& point) const;

  Vector3 applyInverseToPoint(const Vector3& point) const;

  Vector3 applyToNormal(const Vector3& normal) const;

  Vector3 applyInverseToNormal(const Vector3& normal) const;

  Vector3 applyToVector3(const Vector3& vector3) const;

  Vector3 applyInverseToVector3(const Vector3& vector3) const;

  const Matrix4& getMatrix() const;

  const Matrix4& getInverseMatrix() const;

protected:
  Matrix4 matrix;

  Matrix4 inverseMatrix;

  bool outOfDate;
};

#endif // !TRANSFORM_H
