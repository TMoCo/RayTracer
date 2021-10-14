//
// Ray struct declaration
//

#ifndef RAY_H
#define RAY_H

#include <math/Thomath.h>

#include <render/Camera.h>

struct Ray {
public:
  Vector3 _origin;
  Vector3 _direction;

  Ray(const Vector3& o = {}, const Vector3& d = {}) 
    : _origin{o},
      _direction{d}
  {

  }

  inline static Ray generateCameraRay(const Camera& camera, const Vector2& PNDC) // pixel NDC
  {
    F32 tanHalfFOV = std::tan(RADIANS(camera.FOV * 0.5f));
    // a unit direction in -Z
    Vector3 direction{
      (2.0f * PNDC.x - 1.0f) * camera.aspectRatio * tanHalfFOV,
      (2.0f * PNDC.y - 1.0f) * tanHalfFOV,
      -1.0f };
    // rotate direction and set origin
    return { camera.transform.translation, camera.transform.rotatePoint(direction.normalize()) };
  }

  inline Vector3 At(F32 t) const 
  {
    return _origin + t * _direction;
  }
};

#endif // !RAY_H