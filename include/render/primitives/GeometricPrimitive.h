/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// type of primitive for a single geometric shape
//

#ifndef GEOMETRIC_PRIMITIVE_H
#define GEOMETRIC_PRIMITIVE_H 1

#include <render/primitives/Primitive.h>

class Shape;

class GeometricPrimitive : public Primitive
{
  friend class SceneLoader;

public:
  GeometricPrimitive(Shape* shape);

  AABB getBounds() const override;

  bool intersect(const Ray& ray, Surfel* surfel) const override;

  void test();

  Shape* shape;

};

#endif // !GEOMETRIC_PRIMITIVE_H
