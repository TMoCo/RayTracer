/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Camera class declaration
//

#ifndef CAMERA_H
#define CAMERA_H 1

#include <core/core.h>
#include <thomath.h>
#include <cmath>

typedef enum class kCamMove : uint32_t
{
  LEFTWARD,
  RIGHTWARD,
  FORWARD,
  BACKWARD
} kCamMove;

// basic camera class 
class Camera 
{
public:
  Camera();

  Camera(const Vector3& pos, float aspect = 1.0f, float fov = 45.0f, float near = 0.1f, float far = 10.0f);

  Vector3 getCameraVector(const Vector2& PNDC) const;

  Matrix4 getViewMatrix() const;

  Matrix4 getProjectionMatrix() const;

  void processInput(kCamMove move, float speed);

  void update();

  float ar, fov, n, f;

  Vector3 position;

  // updated by window resize
  float vpHeight, vpWidth;

  // updated by mouse movement
  float pitch = 0.0f, yaw = 270.0f;

  Vector3 front = FRONT;
  Vector3 right = RIGHT;
  Vector3 up    = UP;
};

#endif // !CAMERA_H
