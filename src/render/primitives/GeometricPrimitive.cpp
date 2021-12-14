/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <render/primitives/GeometricPrimitive.h>

AABB GeometricPrimitive::getBounds()
{
  return AABB{};
}

bool GeometricPrimitive::intersect(const Ray& ray)
{
  return false;
}

bool GeometricPrimitive::intersectP(const Ray& ray)
{
  return false;
}