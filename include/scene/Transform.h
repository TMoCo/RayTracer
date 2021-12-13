/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
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

  Ray transformRay(const Ray& ray);

  Vector3 transformPoint(const Vector3& point);

  Vector3 transformNormal(const Vector3& normal);

  Vector3 transformVector3(const Vector3& vector3);

  void update();

  // world position rotation and scale
  Vector3 position;

  Quaternion rotation;


  Vector3 scale;

  // matrix representation
  Matrix4 matrix;
  Matrix4 inverseMatrix;
};

#endif // !TRANSFORM_H_
