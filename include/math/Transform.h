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
  Transform(Vector3 position = {}, Quaternion rotation = {}, Vector3 scale = { 1.0f, 1.0f, 1.0f }) : 
    position(position), rotation(rotation)
  {}

  Matrix4 getMatrix();

  void rotate(const Quaternion& quaternion);

  void translate(const Vector3& vec3);

  void scale(const Vector3& scale);

  void lookAt(Transform& target, Vector3 up = UP);

  void update();

  // world position rotation
  Vector3 position;  
  Quaternion rotation;

  // matrix representation
  Matrix4 local;
  Matrix4 invLocal;
  Matrix4 global;
  Matrix4 invGlobal;

};

#endif // !TRANSFORM_H_
