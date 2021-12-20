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
    : origin{ origin }, direction{ direction }, tMax{ tMax }
  { }

  inline static Ray generateCameraRay(const Camera* camera, const Vector2& PNDC)
  {
    Vector3 horizontal = camera->right * camera->vpWidth;
    Vector3 vertical = camera->vpHeight * camera->up;
    Vector3 lowerLeftOrigin = camera->position - horizontal * 0.5f - vertical * 0.5f + camera->front;
    return { camera->position, lowerLeftOrigin + PNDC[0] * horizontal + PNDC[1] * vertical - camera->position, INFINITY };
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
