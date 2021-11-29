//
// type of primitive for a single geometric shape
//

#ifndef GEOMETRIC_PRIMITIVE_H_
#define GEOMETRIC_PRIMITIVE_H_ 1

#include <render/primitives/Primitive.h>
#include <render/shapes/Shape.h>

class GeometricPrimitive : protected Primitive
{
  friend class SceneLoader;
public:
  GeometricPrimitive(Shape* shape) : Primitive(), shape(shape) {}

  AABB getBounds();

  bool intersect(const Ray& ray);

  bool intersectP(const Ray& ray);
  // for computing lighting 
  // virtual const AreaLight* getAreaLight() = 0;

  // virtual const Material* getMaterial() = 0;

protected:
  Shape* shape;
  
  // Material* material;

  // AreaLight* areaLight;
};

#endif // !GEOMETRIC_PRIMITIVE_H_
