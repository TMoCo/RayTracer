/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <render/bounds/AABB.h>
#include <render/primitives/GeometricPrimitive.h>
#include <render/raytracer/Surfel.h>
#include <render/shapes/Shape.h>

GeometricPrimitive::GeometricPrimitive()
  : Primitive(), shape{ nullptr }
{ }


GeometricPrimitive::GeometricPrimitive(Shape* shape)
  : Primitive(), shape{ shape }
{ 
  bounds = new AABB{ shape->getAABB() };
}

GeometricPrimitive::~GeometricPrimitive()
{
  if (bounds)
    delete bounds;
}

const AABB* GeometricPrimitive::getBounds()
{
  return bounds;
}

bool GeometricPrimitive::intersect(const Ray& ray, Surfel* surfel) const
{
  if (!shape->intersect(ray, surfel))
  {
    return false;
  }
  return true;
}

void GeometricPrimitive::draw(Shader* shader) const
{
  // todo: render primitive
}
