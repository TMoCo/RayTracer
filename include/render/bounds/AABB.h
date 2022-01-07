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

#include <Vector3.h>
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

  Vector3 getDiagonal() const;

  Vector3 getOffset(const Vector3& point) const;

  I32 getMaximumExtent() const;

  F32 getSurfaceArea() const;

  bool intersect(const Ray& ray) const;

  AABB& mergeWithAABB(const AABB& other);

  AABB& mergeWithPoint(const Vector3& point);

public:
  Vector3 bounds[2];

};

#endif // !AABB_H
