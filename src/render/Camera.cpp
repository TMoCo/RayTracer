/*
* Author: Thomas Moreno Cooper
* Date created: 07/01/2021
* Copyright 2022 under the MIT license
*/

//
// Camera class definition
//

#include <render/Camera.h>

Camera::Camera()
  : position(0.0f), ar(1.0f), fov(45.0f), n(0.1f), f(10.0f), vpHeight{ 2.0f * tanf(radians(22.5f)) }, 
  vpWidth{ vpHeight }
{}

Camera::Camera(const Vector3& pos, float aspect, float fov, float near, float far)
  : position(pos), ar(aspect), fov(fov), n(near), f(far), vpHeight{ 2.0f * tanf(radians(fov * 0.5f)) }, 
  vpWidth{ vpHeight * aspect }
{}

Vector3 Camera::getCameraVector(const Vector2& PNDC) const
{
  float tanHalffov = tanf(radians(fov * 0.5f));
  return { (2.0f * PNDC[0] - 1.0f) * ar * tanHalffov, (2.0f * PNDC[1] - 1.0f) * tanHalffov, -1.0f };
}

Matrix4 Camera::getViewMatrix() const
{
  return Matrix4::lookAt(position, position + front, up);
}

Matrix4 Camera::getProjectionMatrix() const
{
  return Matrix4::perspective(radians(fov), ar, n, f);
}

void Camera::processInput(kCamMove move, float speed)
{
  switch (move)
  {
  case kCamMove::LEFTWARD:
    position -= right * speed;
    break;
  case kCamMove::RIGHTWARD:
    position += right * speed;
    break;
  case kCamMove::FORWARD:
    position += front * speed;
    break;
  case kCamMove::BACKWARD:
    position -= front * speed;
    break;
  }
}

void Camera::update()
{
  float intPart;
  pitch = modf(pitch, &intPart);
  pitch += (int)intPart % 360;

  front = { cosf(radians(yaw)) * cosf(radians(pitch)), sinf(radians(pitch)), sinf(radians(yaw)) * cosf(radians(pitch)) };
  front.normalize();

  right = pitch > 89.9f && pitch < 269.0f ? front.cross(DOWN).normalize() : front.cross(UP).normalize();

  up = right.cross(front).normalize();
}
