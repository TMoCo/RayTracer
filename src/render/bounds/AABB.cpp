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
  if (t0 > t1) return false;
  // y
  tNear = (bounds[ray.negDir[1]][1] - ray.origin[1]) * ray.inverseDir[1];
  tFar = (bounds[1 - ray.negDir[1]][1] - ray.origin[1]) * ray.inverseDir[1];
  t0 = tNear > t0 ? tNear : t0;
  t1 = tFar < t1 ? tFar : t1;
  if (t0 > t1) return false;
  // z
  tNear = (bounds[ray.negDir[2]][2] - ray.origin[2]) * ray.inverseDir[2];
  tFar = (bounds[1 - ray.negDir[2]][2] - ray.origin[2]) * ray.inverseDir[2];
  t0 = tNear > t0 ? tNear : t0;
  t1 = tFar < t1 ? tFar : t1;
  if (t0 > t1) return false;
  // update
  *tMax = t1;
  return true;
}

AABB AABB::mergeAABB(const AABB& left, const AABB& right)
{
  AABB bbox{};
  
  bbox.bounds[0] = 
  {
    left.bounds[0][0] < right.bounds[0][0] ? left.bounds[0][0] : right.bounds[0][0],
    left.bounds[0][1] < right.bounds[0][1] ? left.bounds[0][1] : right.bounds[0][1] ,
    left.bounds[0][2] < right.bounds[0][2] ? left.bounds[0][2] : right.bounds[0][2] 
  };
  
  bbox.bounds[1] = 
  {
    left.bounds[1][0] > right.bounds[1][0] ? left.bounds[1][0] : right.bounds[1][0],
    left.bounds[1][1] > right.bounds[1][1] ? left.bounds[1][1] : right.bounds[1][1] ,
    left.bounds[1][2] > right.bounds[1][2] ? left.bounds[1][2] : right.bounds[1][2] 
  };

  return bbox;
}

AABB AABB::mergeAABB(const AABB& box, const Vector3& point)
{
  AABB bbox{};

  bbox.bounds[0] = 
  {
    box.bounds[0][0] < point[0] ? box.bounds[0][0] : point[0],
    box.bounds[0][1] < point[1] ? box.bounds[0][1] : point[1] ,
    box.bounds[0][2] < point[2] ? box.bounds[0][2] : point[2] 
  };
  
  bbox.bounds[1] = 
  {
    box.bounds[1][0] > point[0] ? box.bounds[1][0] : point[0],
    box.bounds[1][1] > point[1] ? box.bounds[1][1] : point[1],
    box.bounds[1][2] > point[2] ? box.bounds[1][2] : point[2]
  };
  
  return bbox;
}

AABB AABB::mergeAABB(const Vector3& point, const AABB& box)
{
  return mergeAABB(box, point);
}

AABB AABB::getAABB(const std::vector<Vector3>& positions)
{
  AABB aabb{};

  for (const Vector3& position : positions)
  {
    aabb = mergeAABB(aabb, position);
  }
  return aabb;
}
