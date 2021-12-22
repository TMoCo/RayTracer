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

class AABB;
class Material;
class Surfel;
class Ray;
class Node;

class Primitive
{
public:
  virtual AABB getBounds() const = 0;

  virtual bool intersect(const Ray& ray, Surfel* surfel) const = 0;
  
  virtual void test() = 0;
  
  virtual const Material* getMaterial() const = 0;
};

#endif // !PRIMITIVE_H
