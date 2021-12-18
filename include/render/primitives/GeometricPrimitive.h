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
#include <render/shapes/Shape.h>

class GeometricPrimitive : protected Primitive
{
  friend class SceneLoader;

public:
  GeometricPrimitive(Shape* shape) : Primitive(), shape(shape) {}

  AABB getBounds() const override;

  bool intersect(const Ray& ray, Surfel* surfel) const override;

  bool intersectP(const Ray& ray) const override;

  void test();
  // for computing lighting 
  // virtual const AreaLight* getAreaLight() = 0;

  // virtual const Material* getMaterial() = 0;

  Shape* shape;

protected:
  // Material* material;

  // AreaLight* areaLight;
};

#endif // !GEOMETRIC_PRIMITIVE_H
