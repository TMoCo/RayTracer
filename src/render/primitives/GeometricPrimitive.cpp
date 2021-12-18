/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <render/primitives/GeometricPrimitive.h>

AABB GeometricPrimitive::getBounds() const
{
  return AABB{};
}

bool GeometricPrimitive::intersect(const Ray& ray, Surfel* surfel) const
{
  F32 tHit = ray.tMax;
  if (!shape->intersect(ray, &tHit, surfel))
  {
    return false;
  }
  ray.tMax = tHit;
  return true;
}

bool GeometricPrimitive::intersectP(const Ray& ray) const
{
  return false;
  // TODO: implement shape intersectP methods
  // return shape->intersectP(ray)
}

void GeometricPrimitive::test()
{
}