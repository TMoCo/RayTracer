/*
* Author: Thomas Moreno Cooper
* Date created: 07/01/2022
* Copyright 2022 under the MIT license
*/

#include <Vector3.h>
#include <render/Camera.h>
#include <render/raytracer/Ray.h>

Ray::Ray()
  : tMax{ 0.0f }, negDir{}
{}

Ray::Ray(const Vector3& origin, const Vector3& direction, float tMax)
  : origin{ origin }, direction{ direction.normalize() }, tMax{ tMax },
  inverseDir{ 1.0f / direction }, negDir{ inverseDir[0] < 0.0f, inverseDir[1] < 0.0f, inverseDir[2] < 0.0f }
{}

Ray Ray::generateCameraRay(const Camera* camera, const Vector2& PNDC)
{
  Vector3 h = camera->right * camera->vpWidth;
  Vector3 v = camera->vpHeight * camera->up;
  Vector3 lowerLeftOrigin = camera->position - h * 0.5f - v * 0.5f + camera->front;
  return { camera->position, lowerLeftOrigin + PNDC[0] * h + PNDC[1] * v - camera->position, INFINITY };
}

Vector3 Ray::At(float t) const
{
  return origin + t * direction;
}
