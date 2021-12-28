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

class Material;
class Surfel;
class Ray;
class Node;

class Primitive
{
public:
  virtual bool intersect(const Ray& ray, Surfel* surfel) const = 0;
  
  virtual void test() = 0;
  
  virtual const Material* getMaterial() const
  {
    return material;
  }

  Node* parent;

  Material* material;

  AABB bounds;
};

#endif // !PRIMITIVE_H
