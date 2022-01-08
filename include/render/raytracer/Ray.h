/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#ifndef RAY_H
#define RAY_H 1

#include <thomath.h>

constexpr float tMin = 0.001f;

class Camera;

class Ray
{
public:
  Ray();

  Ray(const Vector3& origin, const Vector3& direction, float tMax);

  static Ray generateCameraRay(const Camera* camera, const Vector2& PNDC);

  Vector3 At(float t) const;
  
  Vector3 origin;

  Vector3 direction;

  Vector3 inverseDir;
  
  int negDir[3];

  mutable float tMax;

};

#endif // !RAY_H
