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

  ~GeometricPrimitive();

  virtual const AABB* getBounds() override;

  virtual bool intersect(const Ray& ray, Surfel* surfel) const override;

  virtual void draw(Shader* shader) const override;

  Shape* shape;

};

#endif // !GEOMETRIC_PRIMITIVE_H
