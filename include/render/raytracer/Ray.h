//
// Ray struct declaration
//

#ifndef RAY_H_
#define RAY_H_

#include <math/thomath.h>

#include <render/Camera.h>

struct Ray {
public:
  Vector3 origin;
  Vector3 direction;
  F32 tMax;

  Ray(const Vector3& o = {}, const Vector3& d = {}) 
    : origin{o}, direction{d}
  {}

  inline static Ray generateCameraRay(const Camera* camera, const Vector2& PNDC) // pixel NDC
  {
    Vector3 horizontal = camera->right * camera->vpWidth;
    Vector3 vertical = camera->vpHeight * camera->up;
    Vector3 lowerLeftOrigin = camera->position - horizontal * 0.5f - vertical * 0.5f + camera->front;
    return { camera->position, lowerLeftOrigin + PNDC.x * horizontal + PNDC.y * vertical - camera->position };
  }

  inline Vector3 At(F32 t) const 
  {
    return origin + t * direction;
  }
};

#endif // !RAY_H_
