/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// primitive interface class
//

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_ 1

#include <render/bounds/AABB.h>
#include <render/raytracer/Ray.h>

class Primitive
{
public:
  virtual AABB getBounds() = 0;

  virtual bool intersect(const Ray& ray) = 0;
  
  virtual bool intersectP(const Ray& ray) = 0;
  
  // virtual const AreaLight* getAreaLight() = 0;
  
  // virtual const Material* getMaterial() = 0;

};

#endif // !PRIMITIVE_H_
