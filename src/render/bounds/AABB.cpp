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

Vector3 AABB::diagonal() const
{
  return bounds[1] - bounds[0];
}

F32 AABB::surfaceArea() const
{
  Vector3 d = diagonal();
  return 2.0f * (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
}

// optional hit arguments updated for positive intersections
bool AABB::intersect(const Ray& ray, F32* tMax) const
{
  F32 t0 = 0.0f, t1 = ray.tMax, tNear, tFar;

  // x
  tNear = (bounds[ray.negDir[0]][0] - ray.origin[0]) * ray.inverseDir[0];
  tFar = (bounds[1 - ray.negDir[0]][0] - ray.origin[0]) * ray.inverseDir[0];
  
  t0 = tNear > t0 ? tNear : t0;
  t1 = tFar < t1 ? tFar : t1;
  
  if (t0 > t1)
  {
    return false;
  }

  // y
  tNear = (bounds[ray.negDir[1]][1] - ray.origin[1]) * ray.inverseDir[1];
  tFar = (bounds[1 - ray.negDir[1]][1] - ray.origin[1]) * ray.inverseDir[1];
  
  t0 = tNear > t0 ? tNear : t0;
  t1 = tFar < t1 ? tFar : t1;
  
  if (t0 > t1)
  {
    return false;
  }

  // z
  tNear = (bounds[ray.negDir[2]][2] - ray.origin[2]) * ray.inverseDir[2];
  tFar = (bounds[1 - ray.negDir[2]][2] - ray.origin[2]) * ray.inverseDir[2];
  
  t0 = tNear > t0 ? tNear : t0;
  t1 = tFar < t1 ? tFar : t1;
  
  if (t0 > t1) 
  {
    return false;
  }

  *tMax = t1;
  return true;
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