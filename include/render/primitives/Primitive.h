/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// primitive interface class
//

#ifndef PRIMITIVE_H
#define PRIMITIVE_H 1

#include <render/bounds/AABB.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>

class Primitive
{
public:
  virtual AABB getBounds() const = 0;

  virtual bool intersect(const Ray& ray, Surfel* surfel) const = 0;
  
  virtual bool intersectP(const Ray& ray) const = 0;

  virtual void test() = 0;
  
  // virtual const AreaLight* getAreaLight() = 0;
  
  // virtual const Material* getMaterial() = 0;

};

#endif // !PRIMITIVE_H
