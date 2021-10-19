//
// Camera class declaration
//

#ifndef CAMERA_H_
#define CAMERA_H_ 1

#include <core/types.h>
#include <core/core.h>

#include <math/thomath.h>

#include <cmath>

// basic camera class 
struct Camera {

  typedef enum : UI32
  {
    LEFTWARD,
    RIGHTWARD,
    FORWARD,
    BACKWARD
  } kMovement;

  F32 aspectRatio;
  F32 FOV;
  F32 zNear;
  F32 zFar;

  Transform transform;

  Vector3 front = FRONT;
  Vector3 right = RIGHT;
  Vector3 up = UP;

  Camera(F32 aspect = 1.0f, F32 fov = 90.0f, F32 near = 0.1f, F32 far = 10.0f) 
    : aspectRatio(aspect), 
      FOV(fov), 
      zNear(near), 
      zFar(far) {}

  inline Vector3 getCameraVector(const Vector2& PNDC) const {
    // fov
    F32 tanHalfFOV = std::tan(RADIANS(FOV * 0.5f));
    // given pixel normalised device coordinates, return a vector in camera's direction
    return {
        (2.0f * PNDC.x - 1.0f) * aspectRatio * tanHalfFOV,
        (2.0f * PNDC.y - 1.0f) * tanHalfFOV,
        -1.0f
    };
  }

  inline Matrix4 getViewMatrix() const
  {
    // build view matrix
    std::cout << front << "\n";
    return Matrix4::lookAt(transform.position, transform.position + front, up);
  }

  inline void processInput(kMovement move, F32 speed)
  {
    // move in camera's direction
    switch (move)
    {
    case LEFTWARD:
      transform.position -= right * speed;
      break;
    case RIGHTWARD:
      transform.position += right * speed;
      break;
    case FORWARD:
      transform.position += front * speed;
      break;
    case BACKWARD:
      transform.position -= front * speed;
      break;
    }
  }
};

#endif // !CAMERA_H_