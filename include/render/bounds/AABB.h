/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// AABB class declaration, built from positions
//

#ifndef AABB_H
#define AABB_H 1

#include <math/thomath.h>
#include <render/raytracer/Ray.h>

#include <glad/glad.h>

#include <array>
#include <limits>
#include <vector>

class AABB
{
public:
  AABB();

  AABB(const Vector3& min, const Vector3& max);

  Vector3 getCentroid() const;

  Vector3 diagonal() const;

  F32 surfaceArea() const;

  AXIS maxExtent() const;

  bool intersect(const Ray& ray, const Vector3& invDir, const I32 negDir[3]) const;

  static AABB mergeAABB(const AABB& left, const AABB& right);

  static AABB mergeAABB(const AABB& box, const Vector3& point);

  static AABB mergeAABB(const Vector3& point, const AABB& box);

  static AABB getAABB(const std::vector<Vector3>& positions);

public:
  Vector3 min;
  Vector3 max;

};

#endif // !AABB_H
