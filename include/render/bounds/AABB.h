//
// AABB from mesh positions
//

#ifndef AABB_H_
#define AABB_H_

#include <render/Mesh.h>

#include <math/thomath.h>

#include <glad/glad.h>

struct AABB
{
  UI32 vao, vbo;

  Vector3 min;
  Vector3 max;
  Vector3 centre;

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

  inline static AABB getAABB(const Mesh& mesh)
  {
    AABB aabb{};
    for (auto& position : mesh.positions)
    {
      aabb.centre += position;
      
      // max
      if (position.x > aabb.max.x)
        aabb.max.x = position.x;
      if (position.y > aabb.max.y)
        aabb.max.y = position.y;
      if (position.z > aabb.max.z)
        aabb.max.z = position.z;

      // min
      if (position.x < aabb.min.x)
        aabb.min.x = position.x;
      if (position.y < aabb.min.y)
        aabb.min.y = position.y;
      if (position.z < aabb.min.z)
        aabb.min.z = position.z;
    }

    aabb.centre /= static_cast<F32>(mesh.positions.size());

    return aabb;
  }

  inline void generateBuffers()
  {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // create vertex data on the fly
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
  
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(F32), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
  }

  inline void draw()
  {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
  }
};

#endif // !AABB_H_
