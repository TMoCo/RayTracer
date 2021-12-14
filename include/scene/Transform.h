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
public:
  Transform(Vector3 position = {}, Quaternion rotation = {}, Vector3 scale = { 1.0f, 1.0f, 1.0f });

  void rotateBy(const Quaternion& quaternion);

  void translateBy(const Vector3& vec3);

  void scaleBy(const Vector3& scale);

  Ray applyToRay(const Ray& ray);

  Ray applyInverseToRay(const Ray& ray);

  Vector3 applyToPoint(const Vector3& point);

  Vector3 applyInverseToPoint(const Vector3& point);

  Vector3 applyToNormal(const Vector3& normal);

  Vector3 applyInverseToNormal(const Vector3& normal);

  Vector3 applyToVector3(const Vector3& vector3);

  Vector3 applyInverseToVector3(const Vector3& vector3);

  void update();

  Vector3 position;

  Quaternion rotation;

  Vector3 scale;

  Matrix4 matrix;

  Matrix4 inverseMatrix;
};

#endif // !TRANSFORM_H_
