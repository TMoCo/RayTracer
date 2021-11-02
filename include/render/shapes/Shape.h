//
// Shape interface class
//

#ifndef SHAPE_H_
#define SHAPE_H_ 1

#include <render/bounds/AABB.h>
#include <render/raytracer/Surfel.h>
#include <render/raytracer/Ray.h>

struct Shape
{
  Shape(bool reverseOrientation) 
    : reverseOrientation(reverseOrientation) {}
  virtual AABB getAABB() const = 0;
  virtual F32 getArea() const = 0;
  inline F32 getPdf() { return 1.0f / getArea(); }
  virtual bool intersect(const Ray& ray, F32* tHit, Surfel* surfel) const = 0;
  inline virtual bool IntersectP(const Ray& ray) const
  {
    F32 tHit = std::numeric_limits<F32>::max();
    Surfel isect;
    return intersect(ray, &tHit, &isect);
  }

  bool reverseOrientation;
};

#endif // !SHAPE_H_