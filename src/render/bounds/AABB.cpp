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
    box.max[0] < point[0] ? box.max[0] : point[0],
    box.max[1] < point[1] ? box.max[1] : point[1],
    box.max[2] < point[2] ? box.max[2] : point[2]
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
    mergeAABB(aabb, position);
  }
  return aabb;
}

void AABB::generateBuffers()
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  // create vertex data on the fly
  std::vector<F32> data = getGlBufferData();

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(F32), data.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

std::vector<F32> AABB::getGlBufferData() const
{
  std::vector<F32> data{};
  data.resize(3 * 8); // 8 * (x,y,z)

  // 0
  data[0] = min[0];
  data[1] = max[1];
  data[2] = min[2];
  // 1
  data[3] = min[0];
  data[4] = max[1];
  data[5] = max[2];
  // 2
  data[6] = max[0];
  data[7] = max[1];
  data[8] = max[2];
  // 3
  data[9] = max[0];
  data[10] = max[1];
  data[11] = min[2];
  // 4
  data[12] = min[0];
  data[13] = min[1];
  data[14] = min[2];
  // 5
  data[15] = min[0];
  data[16] = min[1];
  data[17] = max[2];
  // 6
  data[18] = max[0];
  data[19] = min[1];
  data[20] = max[2];
  // 7
  data[21] = max[0];
  data[22] = min[1];
  data[23] = min[2];

  return data;
}

void AABB::draw() const
{
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
}