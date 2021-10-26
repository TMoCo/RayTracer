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

  // platform agnostic camera movement
  typedef enum : UI32
  {
    LEFTWARD,
    RIGHTWARD,
    FORWARD,
    BACKWARD
  } kMovement;

  // camera data
  F32 aspectRatio;
  F32 FOV;
  F32 zNear;
  F32 zFar;

  // camera position
  Vector3 position;

  // updated by window resize
  F32 vpHeight = 0.0f;
  F32 vpWidth  = 0.0f;

  // updated by mouse movement
  F32 pitch = 0.0f;
  F32 yaw = 270.0f;

  Vector3 front = FRONT;
  Vector3 right = RIGHT;
  Vector3 up = UP;

  Camera(const Vector3& pos, F32 aspect = 1.0f, F32 fov = 90.0f, F32 near = 0.1f, F32 far = 10.0f) 
    : position(pos),
      aspectRatio(aspect),
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
    return Matrix4::lookAt(position, position + front, up);
  }

  inline void processInput(kMovement move, F32 speed)
  {
    // move in camera's direction
    switch (move)
    {
    case LEFTWARD:
      position -= right * speed;
      break;
    case RIGHTWARD:
      position += right * speed;
      break;
    case FORWARD:
      position += front * speed;
      break;
    case BACKWARD:
      position -= front * speed;
      break;
    }
  }

  inline void update()
  {
    F32 intPart;
    pitch = modf(pitch, &intPart);
    pitch += (I32)intPart % 360;

    front = Vector3{
      cosf(RADIANS(yaw)) * cosf(RADIANS(pitch)),
      sinf(RADIANS(pitch)),
      sinf(RADIANS(yaw)) * cosf(RADIANS(pitch)) }.normalize();
    
    right = pitch > 89.9f && pitch < 269.0f ? 
      front.cross(DOWN).normalize() :
      front.cross(UP).normalize();

    up = right.cross(front).normalize();
  }
};

#endif // !CAMERA_H_