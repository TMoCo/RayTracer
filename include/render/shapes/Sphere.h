/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// Sphere primitive
//

#ifndef SPHERE_H
#define SPHERE_H 1

#include <cmath>

#include <render/shapes/Shape.h>

class Sphere : public Shape
{
public:
  Sphere(Transform* toWorld, bool reverseOrientation, F32 radius, F32 zMin, F32 zMax, F32 phiMax)
    : Shape(toWorld, reverseOrientation),
      radius(radius),
      zMin(clamp(fmin(zMin, zMax), -radius, radius)),
      zMax(clamp(fmax(zMin, zMax), -radius, radius)),
      thetaMin(acosf(clamp(zMin / radius, -1.0f, 1.0f))),
      thetaMax(acosf(clamp(zMax / radius, -1.0f, 1.0f))),
      phiMax(radians(clamp(phiMax, 0.0f, 360.0f)))
  {}

  inline AABB getAABB() const override
  {
    // todo: account for varing values of theta
    return AABB{{ -radius, -radius, zMin },
                { radius, radius, zMax } };
  }

  inline bool intersect(const Ray& r, F32* tHit, Surfel* surfel) const override
  {
    Ray ray = toWorld->applyInverseToRay(r); // world space ray to object space

    // TODO: use running error analysis to bound error, use error bounds of transforming from world to object
    F32 a = ray.direction.dot(ray.direction);
    F32 b = 2.0f * (ray.direction.dot(ray.origin));
    F32 c = ray.origin.dot(ray.origin) - radius * radius;

    F32 t0, t1;
    if (!quadratic(a, b, c, &t0, &t1))
    {
      return false;
    }

    if (t0 > ray.tMax || t1 <= 0.0f)
    {
      return false; // intersection is outside of range
    }

    F32 t = t0;
    if (t <= 0.0f)
    {
      t = t1;
      if (t > ray.tMax)
      {
        return false;
      }
    }

    return true;
  }

  inline F32 getArea() const override
  {
    return (radius * radius) * FOUR_PI;
  }

private:
  F32 radius;
  
  F32 zMin, zMax;
  
  F32 thetaMin, thetaMax, phiMax;
};

#endif // !SPHERE_H
