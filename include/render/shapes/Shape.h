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

#include <render/primitives/Primitive.h>
#include <render/bounds/AABB.h>
#include <render/raytracer/Ray.h>
#include <scene/Transform.h>

class Surfel;

class Shape
{
public:
  Shape(Transform* toWorld) 
    : toWorld{ toWorld }, primitive{ nullptr }
  { }

  virtual AABB getAABB() const = 0;

  virtual F32 getArea() const = 0;

  virtual F32 getPdf() const { return 1.0f / getArea(); }

  virtual bool intersect(const Ray& ray, Surfel* surfel) const = 0;

  Transform* toWorld;

  Primitive* primitive;

};

#endif // !SHAPE_H