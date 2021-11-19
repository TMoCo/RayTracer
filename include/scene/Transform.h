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

  void rotate(const Quaternion& quaternion);

  void translate(const Vector3& vec3);

  void scale(const Vector3& scale);

  Ray transformRay(const Ray& ray);

  Vector3 transformPoint(const Vector3& point);

  Vector3 transformNormal(const Vector3& normal);

  Vector3 transformVector3(const Vector3& vector3);

  // world position rotation
  Vector3 position;

  Quaternion rotation;

  // matrix representation
  Matrix4 matrix;
  Matrix4 inverseMatrix;

};

#endif // !TRANSFORM_H_
