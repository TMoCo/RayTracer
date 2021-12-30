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
#include <render/Shader.h>

class Material;
class Surfel;
class Ray;
class Node;

class Primitive
{
public:
  Primitive() 
    : parent{ nullptr }, material{ nullptr }, bounds { nullptr }
  { }

  ~Primitive()
  {
    if (bounds)
    {
      delete bounds;
    }
  }

  virtual const AABB* getBounds() = 0;

  virtual bool intersect(const Ray& ray, Surfel* surfel) const = 0;
  
  virtual const Material* getMaterial() const
  {
    return material;
  }

  virtual void draw(Shader* shader) const = 0;

  Node* parent;

  Material* material;

  AABB* bounds;
};

#endif // !PRIMITIVE_H
