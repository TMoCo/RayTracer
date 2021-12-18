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
  GeometricPrimitive(Shape* shape, Material* material, AreaLight* areaLight);

  AABB getBounds() const override;

  bool intersect(const Ray& ray, Surfel* surfel) const override;

  bool intersectP(const Ray& ray) const override;

  void test();

  const AreaLight* getAreaLight() const override;

  const Material* getMaterial() const override;

protected:
  Shape* shape;

  Material* material;

  AreaLight* areaLight;
};

#endif // !GEOMETRIC_PRIMITIVE_H
