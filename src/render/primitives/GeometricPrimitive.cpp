/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <core/debug.h>
#include <render/bounds/AABB.h>
#include <render/primitives/GeometricPrimitive.h>
#include <render/raytracer/Surfel.h>
#include <render/shapes/Shape.h>

GeometricPrimitive::GeometricPrimitive( Shape* shape, Material* material)
  : Primitive(), shape{ shape }, material{ material }
{ }

AABB GeometricPrimitive::getBounds() const
{
  return shape->getAABB();
}

bool GeometricPrimitive::intersect(const Ray& ray, Surfel* surfel) const
{
  F32 tHit = ray.tMax;
  if (!shape->intersect(ray, &tHit, surfel))
  {
    return false;
  }
  ray.tMax = tHit;
  surfel->material = material;
  return true;
}

void GeometricPrimitive::test()
{
  DEBUG_PRINT("%s", "Test function...\n");
}

const Material* GeometricPrimitive::getMaterial() const
{
  return material;
}