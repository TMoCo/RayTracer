/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Ray struct declaration
//

#ifndef RAY_H
#define RAY_H 1

#include <Vector3.h>
#include <render/Camera.h>

constexpr float tMin = 0.001f;

class Ray
{
public:
  Ray() 
    : tMax{ 0.0f }, negDir{}
  {}

  Ray(const Vector3& origin, const Vector3& direction, float tMax)
    : origin{ origin }, direction{ direction.normalize() }, tMax{ tMax },
    inverseDir{ 1.0f / direction }, negDir{ inverseDir[0] < 0.0f, inverseDir[1] < 0.0f, inverseDir[2] < 0.0f }
  { }

  inline static Ray generateCameraRay(const Camera* camera, const Vector2& PNDC)
  {
    Vector3 h = camera->right * camera->vpWidth;
    Vector3 v = camera->vpHeight * camera->up;
    Vector3 lowerLeftOrigin = camera->position - h * 0.5f - v * 0.5f + camera->front;
    return { camera->position, lowerLeftOrigin + PNDC[0] * h + PNDC[1] * v - camera->position, INFINITY };
  }

  inline Vector3 At(float t) const 
  {
    return origin + t * direction;
  }
  
  Vector3 origin;

  Vector3 direction;

  Vector3 inverseDir;
  
  int negDir[3];

  mutable float tMax;

};

#endif // !RAY_H
