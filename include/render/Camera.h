/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// Camera class declaration
//

#ifndef CAMERA_H
#define CAMERA_H 1

#include <core/types.h>
#include <core/core.h>

#include <thomath.h>

#include <cmath>

// basic camera class 
struct Camera {

  // platform agnostic camera movement
  typedef enum : uint32_t
  {
    LEFTWARD,
    RIGHTWARD,
    FORWARD,
    BACKWARD
  } kMovement;

  // camera data
  float aspectRatio;
  float FOV;
  float zNear;
  float zFar;

  // camera position
  Vector3 position;

  // updated by window resize
  float vpHeight = 0.0f;
  float vpWidth  = 0.0f;

  // updated by mouse movement
  float pitch = 0.0f;
  float yaw = 270.0f;

  Vector3 front = FRONT;
  Vector3 right = RIGHT;
  Vector3 up = UP;

  Camera(const Vector3& pos, float aspect = 1.0f, float fov = 45.0f, float near = 0.1f, float far = 10.0f) 
    : position(pos),
      aspectRatio(aspect),
      FOV(fov), 
      zNear(near), 
      zFar(far) {}

  inline Vector3 getCameraVector(const Vector2& PNDC) const {
    // fov
    float tanHalfFOV = std::tan(radians(FOV * 0.5f));
    // given pixel normalised device coordinates, return a vector in camera's direction
    return {
        (2.0f * PNDC[0] - 1.0f) * aspectRatio * tanHalfFOV,
        (2.0f * PNDC[1] - 1.0f) * tanHalfFOV,
        -1.0f
    };
  }

  inline Matrix4 getViewMatrix() const
  {
    // build view matrix
    return Matrix4::lookAt(position, position + front, up);
  }

  inline Matrix4 getProjectionMatrix() const
  {
    return Matrix4::perspective(radians(FOV), aspectRatio, zNear, zFar);
  }

  inline void processInput(kMovement move, float speed)
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
    float intPart;
    pitch = modf(pitch, &intPart);
    pitch += (int)intPart % 360;

    front = Vector3{
      cosf(radians(yaw)) * cosf(radians(pitch)),
      sinf(radians(pitch)),
      sinf(radians(yaw)) * cosf(radians(pitch)) }.normalize();
    
    right = pitch > 89.9f && pitch < 269.0f ? front.cross(DOWN).normalize() : front.cross(UP).normalize();

    up = right.cross(front).normalize();
  }
};

#endif // !CAMERA_H
