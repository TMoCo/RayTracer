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
  : min{ std::numeric_limits<F32>::max() }, max{ std::numeric_limits<F32>::lowest() }
{ }

AABB::AABB(const Vector3& min, const Vector3& max) 
  : min{ min }, max{ max }
{ }

Vector3 AABB::getCentroid() const
{
  return (min + max) * 0.5f;
}

Vector3 AABB::diagonal() const
{
  return max - min;
}

F32 AABB::surfaceArea() const
{
  Vector3 d = diagonal();
  return 2.0f * (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
}

AXIS AABB::maxExtent() const
{
  Vector3 d = diagonal();
  if (d[0] > d[1] && d[0] > d[2])
  {
    return AXIS::X;
  }
  else if (d[1] > d[2])
  {
    return AXIS::Y;
  }
  else
  {
    return AXIS::Z;
  }
}

// optional hit arguments updated for positive intersections
bool AABB::intersect(const Ray& ray, const Vector3& invDir, const I32 negDir[3]) const
{
  return false;
}

AABB AABB::mergeAABB(const AABB& left, const AABB& right)
{
  AABB bbox{};
  
  bbox.min = 
  {
    left.min[0] < right.min[0] ? left.min[0] : right.min[0],
    left.min[1] < right.min[1] ? left.min[1] : right.min[1] ,
    left.min[2] < right.min[2] ? left.min[2] : right.min[2] 
  };
  
  bbox.max = 
  {
    left.max[0] > right.max[0] ? left.max[0] : right.max[0],
    left.max[1] > right.max[1] ? left.max[1] : right.max[1] ,
    left.max[2] > right.max[2] ? left.max[2] : right.max[2] 
  };

  return bbox;
}

AABB AABB::mergeAABB(const AABB& box, const Vector3& point)
{
  AABB bbox{};

  bbox.min = 
  {
    box.min[0] < point[0] ? box.min[0] : point[0],
    box.min[1] < point[1] ? box.min[1] : point[1] ,
    box.min[2] < point[2] ? box.min[2] : point[2] 
  };
  
  bbox.max = 
  {
    box.max[0] > point[0] ? box.max[0] : point[0],
    box.max[1] > point[1] ? box.max[1] : point[1],
    box.max[2] > point[2] ? box.max[2] : point[2]
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
