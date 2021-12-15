/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Shape interface class
//

#ifndef SHAPE_H
#define SHAPE_H 1

#include <render/bounds/AABB.h>

#include <render/raytracer/Ray.h>

#include <scene/Transform.h>

class Surfel;

class Shape
{
public:
  Shape(Transform* toWorld, bool reverseOrientation) : reverseOrientation(reverseOrientation), toWorld(toWorld) {}

  virtual AABB getAABB() const = 0;

  virtual F32 getArea() const = 0;

  inline F32 getPdf() { return 1.0f / getArea(); }

  virtual bool intersect(const Ray& ray, F32* tHit, Surfel* surfel) const = 0;

  Transform* toWorld;

  bool reverseOrientation;
};

#endif // !SHAPE_H