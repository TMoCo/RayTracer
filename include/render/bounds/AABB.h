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

  void generateBuffers();

  std::vector<F32> getGlBufferData() const;

  void draw() const;

  static AABB mergeAABB(const AABB& left, const AABB& right);

  static AABB mergeAABB(const AABB& box, const Vector3& point);

  static AABB mergeAABB(const Vector3& point, const AABB& box);

  static AABB getAABB(const std::vector<Vector3>& positions);

public:
  UI32 vao, vbo;

  Vector3 min;
  Vector3 max; // note: min() = 0!

  /*
  vertex 2 = max
  vertex 4 = min

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
        O-------x
       /
      /
     z
  */

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

  static constexpr const Vector3 unitCube[8] =
  {
    { -1.0f, 1.0f, 1.0f },
    { -1.0f, 1.0f, -1.0f },
    { 1.0f, 1.0f, -1.0f },
    { 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, 1.0f },
    { -1.0f, -1.0f, -1.0f },
    { 1.0f, -1.0f, -1.0f },
    { 1.0f, -1.0f, 1.0f },
  };


};

#endif // !AABB_H
