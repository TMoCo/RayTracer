//
// Transform class declaration
//

#ifndef TRANSFORM_H_
#define TRANSFORM_H_ 1

#include <math/Matrix4.h>
#include <math/Quaternion.h>
#include <math/Vector3.h>

class Transform {
public:
  Transform(Vector3 position = {}, Quaternion rotation = {}, Vector3 scale = { 1.0f, 1.0f, 1.0f });

  void rotate(const Quaternion& quaternion);

  void translate(const Vector3& vec3);

  void scale(const Vector3& scale);

  // world position rotation
  Vector3 position;

  Quaternion rotation;

  // matrix representation
  Matrix4 matrix;
  Matrix4 inverseMatrix;

};

#endif // !TRANSFORM_H_
