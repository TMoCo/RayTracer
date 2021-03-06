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
  Sphere(Transform* toWorld, float radius)
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

  inline bool intersect(const rt::Ray& ray, Surfel* surfel) const override
  {
    float t;
    {
      Vector3 originCentre = ray.origin - toWorld->getTranslation();
      float a = ray.direction.lengthSquared();
      float b = 2.0f * (originCentre.dot(ray.direction));
      float c = originCentre.lengthSquared() - radius * radius;

      float t0, t1;
      if (!quadratic(a, b, c, &t0, &t1))
      {
        return false; // no intersection
      }

      t = t0 <= rt::RAY_T_MIN ? t1 : t0; // smallest 

      if (t1 <= rt::RAY_T_MIN || t0 > ray.tMax || t > ray.tMax)
      {
        return false; // invalid intersection
      }
    }

    Vector3 pHit = ray.at(t);
    Vector3 normal = (pHit - toWorld->getTranslation()) / radius;
    float pPhi, pTheta;
    pTheta = acosf(-normal[1]);
    pPhi = atan2f(-normal[2], normal[0]) + PI;
    Vector2 uv = { pPhi * REC_TWO_PI, pTheta * REC_PI }; // map from UV range [0,2pi] to [0,1]
    

    *surfel = { pHit, normal, uv, -ray.direction, this };
    surfel->tangent = Vector3{ -sinf(uv[0]), 0.0f, cosf(uv[0]) }.normalize();
    surfel->material = primitive->material;
    ray.tMax = t;

    return true;
  }

  inline float getArea() const override
  {
    return (radius * radius) * FOUR_PI;
  }

private:
  float radius;
};

#endif // !SPHERE_H
