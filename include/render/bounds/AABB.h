//
// AABB from mesh positions
//

#ifndef AABB_H_
#define AABB_H_

#include <render/Mesh.h>

#include <math/thomath.h>

#include <glad/glad.h>

#include <limits>

struct AABB
{
  UI32 vao, vbo;

  Vector3 min;
  Vector3 max; // note: min() = 0!

  // box indices
  static constexpr const UI32 indices[36] = 
  {
    0, 2, 3,
    1, 2, 0,
    1, 2, 6,
    6, 5, 1,
    2, 3, 6,
    7, 6, 3,
    3, 0, 7,
    7, 0, 4,
    6, 7, 5,
    7, 4, 5,
    0, 1, 4,
    4, 1, 5
  };

  AABB()
    : min(std::numeric_limits<F32>::max()), max(std::numeric_limits<F32>::lowest())
  {}

  AABB(const Vector3& min, const Vector3& max) 
    : min(std::numeric_limits<F32>::max()), max(std::numeric_limits<F32>::lowest()) 
  {
    // test min
    if (this->min.x > min.x)
      this->min.x = min.x;
    if (this->min.y > min.y)
      this->min.y = min.y;
    if (this->min.z > min.z)
      this->min.z = min.z;

    // test max
    if (this->max.x < max.x)
      this->max.x = max.x;
    if (this->max.y < max.y)
      this->max.y = max.y;
    if (this->max.z < max.z)
      this->max.z = max.z;
  }

  inline Vector3 getCentroid() const
  {
    return (min + max) * 0.5f;
  }

  inline Vector3 diagonal() const
  {
    return max - min;
  }

  inline F32 surfaceArea() const
  {
    Vector3 d = diagonal();
    return 2.0f * (d.x * d.y + d.x * d.z + d.y * d.z);
  }

  inline AXIS maxExtent() const
  {
    Vector3 d = diagonal();
    if (d.x > d.y && d.x > d.z)
      return AXIS::X;
    else if (d.y > d.z)
      return AXIS::Y;
    else
      return AXIS::Z;
  }

  // optional hit arguments updated for positive intersections
  inline bool intersect(const Ray& ray, const Vector3& invDir, const I32 negDir[3]) const
  {
    F32 tMin = (bounds[dirIsNeg[0]].x - ray.o.x) * invDir.x;
    F32 tMax = (bounds[1 - dirIsNeg[0]].x - ray.o.x) * invDir.x;
    F32 tyMin = (bounds[dirIsNeg[1]].y - ray.o.y) * invDir.y;
    F32 tyMax = (bounds[1 - dirIsNeg[1]].y - ray.o.y) * invDir.y;

  }

  inline static AABB mergeAABB(const AABB& left, const AABB& right)
  {
    AABB bbox{};
    bbox.min = {
      left.min.x < right.min.x ? left.min.x : right.min.x,
      left.min.y < right.min.y ? left.min.y : right.min.y ,
      left.min.z < right.min.z ? left.min.z : right.min.z};
    bbox.max = {
      left.max.x > right.max.x ? left.max.x : right.max.x,
      left.max.y > right.max.y ? left.max.y : right.max.y ,
      left.max.z > right.max.z ? left.max.z : right.max.z };
    return bbox;
  }

  inline static AABB mergeAABB(const AABB& box, const Vector3& point)
  {
    AABB bbox{};
    bbox.min = Vector3{
      box.min.x < point.x ? box.min.x : point.x,
      box.min.y < point.y ? box.min.y : point.y ,
      box.min.z < point.z ? box.min.z : point.z };
    bbox.max = Vector3{
      box.max.x < point.x ? box.max.x : point.x,
      box.max.y < point.y ? box.max.y : point.y,
      box.max.z < point.z ? box.max.z : point.z
    };
    return bbox;
  }

  inline static AABB mergeAABB(const Vector3& point, const AABB& box)
  {
    return mergeAABB(box, point);
  }

  inline static AABB getAABB(const std::vector<Vector3>& positions)
  {
    AABB aabb{};
    for (auto& position : positions)
      mergeAABB(aabb, position);
    return aabb;
  }

  inline static AABB getAABB(const Mesh& mesh)
  {
    return getAABB(mesh.positions);
  }

  inline void generatebuffers()
  {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // create vertex data on the fly
    std::vector<F32> data = getGlbufferData();
  
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(F32), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
  }

  inline std::vector<F32> getGlbufferData()
  {
    std::vector<F32> data{};
    data.resize(3 * 8); // 8 * (x,y,z)

    /*
        1-----------2
       /|          /|
      / |         / |
     /  |        /  |
    0-----------3   |
    |   |       |   |
    |   5-------|---6
    |  /        |  /
    | /         | /
    |/          |/
    4-----------7

          y
          |
          |
          |_______x
         /
        /
       z

    4 = min
    2 = max
    */


    // 0
    data[0] = min.x;
    data[1] = max.y;
    data[2] = min.z;
    // 1
    data[3] = min.x;
    data[4] = max.y;
    data[5] = max.z;
    // 2
    data[6] = max.x;
    data[7] = max.y;
    data[8] = max.z;
    // 3
    data[9] = max.x;
    data[10] = max.y;
    data[11] = min.z;
    // 4
    data[12] = min.x;
    data[13] = min.y;
    data[14] = min.z;
    // 5
    data[15] = min.x;
    data[16] = min.y;
    data[17] = max.z;
    // 6
    data[18] = max.x;
    data[19] = min.y;
    data[20] = max.z;
    // 7
    data[21] = max.x;
    data[22] = min.y;
    data[23] = min.z;

    return data;
  }

  inline void draw()
  {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
  }
};

#endif // !AABB_H_