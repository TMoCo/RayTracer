/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <render/primitives/GeometricPrimitive.h>
#include <render/raytracer/Surfel.h>

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
  F32 tMax = 100.0f;
  Surfel surfaceInteraction;
  return shape->intersect(ray, &tMax, &surfaceInteraction);
}

void GeometricPrimitive::test()
{
  std::cout << "shape inverse matrix:\n" << shape->toWorld->getInverseMatrix() << "\n\n";
}