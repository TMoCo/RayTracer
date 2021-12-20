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

#include <math/thomath.h>
#include <render/Camera.h>

class Ray {
public:
  Ray() : tMax{ 0.0f } 
  { }

  Ray(const Vector3& origin, const Vector3& direction, F32 tMax) 
    : origin{ origin }, direction{ direction.normalize() }, tMax{ tMax }
  { }

  inline static Ray generateCameraRay(const Camera* camera, const Vector2& PNDC)
  {
    Vector3 h = camera->right * camera->vpWidth;
    Vector3 v = camera->vpHeight * camera->up;
    Vector3 lowerLeftOrigin = camera->position - h * 0.5f - v * 0.5f + camera->front;
    return { camera->position, lowerLeftOrigin + PNDC[0] * h + PNDC[1] * v - camera->position, INFINITY };
  }

  inline Vector3 At(F32 t) const 
  {
    return origin + t * direction;
  }
  
  Vector3 origin;

  Vector3 direction;
  
  mutable F32 tMax;
};

#endif // !RAY_H
