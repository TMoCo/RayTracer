/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// Quad primitive struct
//

#ifndef QUAD_H
#define QUAD_H 1

#include <math/thomath.h>

#include <render/shapes/shape.h>

#include <glad/glad.h>

#include <vector>

struct Quad : Shape
{
  static constexpr const UI32 indices[6] =
  {
    0, 1, 3,
    1, 2, 3
  };

  // overload 
  AABB getAABB() const
  {

  }

  F32 getArea() const
  {

  }

  bool intersect(const Ray& ray, F32* tHit, Surfel* surfel) const
  {

  }

  inline static Quad createQuad(
    const Vector2& xExtents = { -0.5f, 0.5f }, 
    const Vector2& zExtents = { -0.5f, 0.5f })
  {
    Quad quad{};

    glGenVertexArrays(1, &quad.vao);
    glGenBuffers(1, &quad.vbo);
    
    std::vector<F32> data{};
    data.reserve(6 * 3);

    // top right
    data.push_back(xExtents.y);
    data.push_back(0.0f);
    data.push_back(zExtents.y);
    // bottom right
    data.push_back(xExtents.y);
    data.push_back(0.0f);
    data.push_back(zExtents.x);
    // bottom left
    data.push_back(xExtents.x);
    data.push_back(0.0f);
    data.push_back(zExtents.x);
    // top left
    data.push_back(xExtents.x);
    data.push_back(0.0f);
    data.push_back(zExtents.y);

    glBindVertexArray(quad.vao);

    glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(F32), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    return quad;
  }

  inline void draw()
  {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
  }

  UI32 vao, vbo;
};

#endif