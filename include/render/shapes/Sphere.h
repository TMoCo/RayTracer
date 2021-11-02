//
// Sphere primitive
//

#ifndef SPHERE_H_
#define SPHERE_H_ 1

#include <render/shapes/Shape.h>

struct Sphere : Shape
{
  Sphere(bool reverseOrientation, F32 radius, F32 zMin, F32 zMax, F32 phiMax)
    : Shape(reverseOrientation),
      radius(radius),
      zMin(clamp(fmin(zMin, zMax), -radius, radius)),
      zMax(clamp(fmax(zMin, zMax), -radius, radius)),
      thetaMin(acosf(clamp(zMin / radius, -1.0f, 1.0f))),
      thetaMax(acosf(clamp(zMax / radius, -1.0f, 1.0f))),
      phiMax(radians(clamp(phiMax, 0.0f, 360.0f)))
  {}

  inline AABB getAABB()
  {
    // todo: account for varing values of theta
    return AABB{{ -radius, -radius, zMin },
                { radius, radius, zMax } };
  }

  F32 radius;
  F32 zMin, zMax;
  F32 thetaMin, thetaMax, phiMax;
};

#endif // !SPHERE_H_