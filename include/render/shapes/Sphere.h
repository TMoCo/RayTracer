/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Sphere primitive
//

#ifndef SPHERE_H
#define SPHERE_H 1


#include <render/shapes/Shape.h>
#include <render/raytracer/Surfel.h>

#include <cmath>

class Sphere : public Shape
{
public:
  Sphere(Transform* toWorld, F32 radius)
    : Shape(toWorld), radius(radius)
  { }

  inline AABB getAABB() const override
  {
    return 
    { 
      Vector3{ -radius, -radius, -radius } + toWorld->getTranslation(), 
      Vector3{ radius, radius, radius } + toWorld->getTranslation() 
    };
  }

  inline bool intersect(const Ray& ray, F32* tHit, Surfel* surfel) const override
  {
    F32 t;
    {
      Vector3 originCentre = ray.origin - toWorld->getTranslation();
      F32 a = ray.direction.lengthSquared();
      F32 b = 2.0f * (originCentre.dot(ray.direction));
      F32 c = originCentre.lengthSquared() - radius * radius;

      F32 t0, t1;
      if (!quadratic(a, b, c, &t0, &t1))
      {
        return false; // no intersection
      }

      t = t0 <= tMin ? t1 : t0; // smallest 

      if (t1 <= tMin || t0 > ray.tMax || t > ray.tMax)
      {
        return false;
      }
    }

    Vector3 pHit = ray.At(t);
    Vector3 normal = (pHit - toWorld->getTranslation()) / radius;
    F32 pPhi, pTheta;
    pTheta = acosf(-normal[1]);
    pPhi = atan2f(-normal[2], normal[0]) + PI;
    Vector2 uv = { pPhi * REC_TWO_PI, pTheta * REC_PI }; // map from UV range [0,2pi] to [0,1]
    

    *surfel = { pHit, normal, uv, -ray.direction, this };
    surfel->tangent = Vector3{ -sinf(uv[0]), 0.0f, cosf(uv[0]) }.normalize();
    *tHit = t;

    return true;
  }

  inline F32 getArea() const override
  {
    return (radius * radius) * FOUR_PI;
  }

private:
  F32 radius;
};

#endif // !SPHERE_H
