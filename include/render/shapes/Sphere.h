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
#include <render/raytracer/Surfel.h>

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
    F32 t;

    {
      F32 a = ray.direction.dot(ray.direction);
      F32 b = 2.0f * (ray.direction.dot(ray.origin));
      F32 c = ray.origin.dot(ray.origin) - radius * radius;

      F32 t0, t1;
      if (!quadratic(a, b, c, &t0, &t1))
      {
        return false; // no intersection
      }

      t = t0 <= 0.0f ? t1 : t0; // smallest 

      if (t1 <= 0.0f || t0 > ray.tMax || t > ray.tMax)
      {
        return false;
      }
    }

    F32 pPhi, pTheta;
    Vector3 pHit = ray.At(t);

    /*
    pTheta = acosf(clamp(pHit[2] / radius, -1.0f, 1.0f));
    pPhi = atan2f(pHit[1], pHit[0]);
    pPhi += (F32)(pPhi < 0.0f) * TWO_PI;
    Vector2 uv = { pPhi / phiMax, (pTheta - thetaMin) / (thetaMax - thetaMin) }; // map from UV range [0,2pi] to [0,1]
    */
    
    *surfel = toWorld->applyToSurfel({ pHit, pHit / radius, -ray.direction, this });
    *tHit = t;

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
