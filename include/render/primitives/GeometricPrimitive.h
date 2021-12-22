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

class GeometricPrimitive : protected Primitive
{
  friend class SceneLoader;

public:
  GeometricPrimitive(Shape* shape, Material* material);

  AABB getBounds() const override;

  bool intersect(const Ray& ray, Surfel* surfel) const override;

  void test();

  const Material* getMaterial() const override;

  Shape* shape;

  Material* material;

};

#endif // !GEOMETRIC_PRIMITIVE_H
