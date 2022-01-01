/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 21/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// AABB class definition
//

#include <render/bounds/AABB.h>

AABB::AABB() 
  : bounds{ std::numeric_limits<F32>::max(), std::numeric_limits<F32>::lowest() }
{ }

AABB::AABB(const Vector3& min, const Vector3& max) 
  : bounds{ min, max }
{ }

Vector3 AABB::getCentroid() const
{
  return (bounds[0] + bounds[1]) * 0.5f;
}

Vector3 AABB::getDiagonal() const
{
  return bounds[1] - bounds[0];
}

Vector3 AABB::getOffset(const Vector3& point) const
{
  Vector3 offset = point - bounds[0];
  if (bounds[1][0] > bounds[0][0])
  {
    offset[0] /= bounds[1][0] - bounds[0][0];
  }

  if (bounds[1][1] > bounds[0][1])
  {
    offset[1] /= bounds[1][1] - bounds[0][1];
  }

  if (bounds[1][2] > bounds[0][2])
  {
    offset[2] /= bounds[1][2] - bounds[0][2];
  }
  return offset;
}

I32 AABB::getMaximumExtent() const
{
  Vector3 diagonal = getDiagonal();
  if (diagonal[0] > diagonal[1] && diagonal[0] > diagonal[2])
  {
    return 0;
  }
  else if (diagonal[1] > diagonal[2])
  {
    return 1;
  }
  else
  {
    return 2;
  }

}

F32 AABB::getSurfaceArea() const
{
  Vector3 d = getDiagonal();
  return 2.0f * (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
}

// optional hit arguments updated for positive intersections
bool AABB::intersect(const Ray& ray) const
{
  // x
  F32 tMin = (bounds[ray.negDir[0]][0] - ray.origin[0]) * ray.inverseDir[0];
  F32 tMax = (bounds[1 - ray.negDir[0]][0] - ray.origin[0]) * ray.inverseDir[0];
  F32 tMinY = (bounds[ray.negDir[1]][1] - ray.origin[1]) * ray.inverseDir[1];
  F32 tMaxY = (bounds[1 - ray.negDir[1]][1] - ray.origin[1]) * ray.inverseDir[1];
  
  if (tMin > tMaxY || tMinY > tMax)
  {
    return false;
  }

  if (tMinY > tMin)
  {
    tMin = tMinY;
  }

  if (tMaxY < tMax)
  {
    tMax = tMaxY;
  }

  // z
  F32 tMinZ = (bounds[ray.negDir[2]][2] - ray.origin[2]) * ray.inverseDir[2];
  F32 tMaxZ = (bounds[1 - ray.negDir[2]][2] - ray.origin[2]) * ray.inverseDir[2];
  
  if (tMin > tMaxZ || tMinZ > tMax)
  {
    return false;
  }

  if (tMinZ > tMin)
  {
    tMin = tMinZ;
  }

  if (tMaxZ < tMax)
  {
    tMax = tMaxZ;
  }

  return (tMin < ray.tMax) && (tMax > 0);
}

AABB& AABB::mergeWithAABB(const AABB& other)
{
  bounds[0] =
  {
    bounds[0][0] < other.bounds[0][0] ? bounds[0][0] : other.bounds[0][0],
    bounds[0][1] < other.bounds[0][1] ? bounds[0][1] : other.bounds[0][1],
    bounds[0][2] < other.bounds[0][2] ? bounds[0][2] : other.bounds[0][2]
  };

  bounds[1] =
  {
    bounds[1][0] > other.bounds[1][0] ? bounds[1][0] : other.bounds[1][0],
    bounds[1][1] > other.bounds[1][1] ? bounds[1][1] : other.bounds[1][1],
    bounds[1][2] > other.bounds[1][2] ? bounds[1][2] : other.bounds[1][2]
  };

  return *this;
}

AABB& AABB::mergeWithPoint(const Vector3& point)
{
  bounds[0] =
  {
    bounds[0][0] < point[0] ? bounds[0][0] : point[0],
    bounds[0][1] < point[1] ? bounds[0][1] : point[1] ,
    bounds[0][2] < point[2] ? bounds[0][2] : point[2]
  };

  bounds[1] =
  {
    bounds[1][0] > point[0] ? bounds[1][0] : point[0],
    bounds[1][1] > point[1] ? bounds[1][1] : point[1],
    bounds[1][2] > point[2] ? bounds[1][2] : point[2]
  };

  return *this;
}