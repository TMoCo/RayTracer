/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <core/debug.h>
#include <render/primitives/Mesh.h>
#include <render/bounds/AABB.h>

Mesh::Mesh()
  : Primitive(), VAO{ 0 }, VBO{ 0 }, EBO{ 0 }
{ }

AABB Mesh::getBounds() const
{
  return AABB::getAABB(positions);
}

bool Mesh::intersect(const Ray& ray, Surfel* surfel) const 
{
  return false;
}

void Mesh::test()
{
  DEBUG_PRINT("Testing\n");
}

const Material* Mesh::getMaterial() const
{
  return nullptr;
}

void Mesh::generateBuffers(bool interleave)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  std::vector<F32> data; // prepare mesh data to send to GPU
  size_t nVertices = positions.size(); // assumes at least positions
  data.reserve(nVertices * 3);
  if (normals.size() > 0)
  {
    data.reserve(data.capacity() + nVertices * 3);
  }
  if (textureCoords.size() > 0)
  {
    data.reserve(data.capacity() + nVertices * 2);
  }

  if (interleave)
  {
    // todo: find better way to interleave data
    for (UI32 i = 0; i < nVertices; ++i)
    {
      data.push_back(positions[i][0]);
      data.push_back(positions[i][1]);
      data.push_back(positions[i][2]);

      if (normals.size() > 0)
      {
        data.push_back(normals[i][0]);
        data.push_back(normals[i][1]);
        data.push_back(normals[i][2]);
      }

      if (textureCoords.size() > 0)
      {
        data.push_back(textureCoords[i][0]);
        data.push_back(textureCoords[i][1]);
      }
    }
  }
  else
  {
    for (UI32 i = 0; i < nVertices; ++i)
    {
      data.push_back(positions[i][0]);
      data.push_back(positions[i][1]);
      data.push_back(positions[i][2]);
    }

    if (normals.size() > 0)
    {
      for (UI32 i = 0; i < nVertices; ++i)
      {
        data.push_back(normals[i][0]);
        data.push_back(normals[i][1]);
        data.push_back(normals[i][2]);
      }
    }

    if (textureCoords.size() > 0)
    {
      for (UI32 i = 0; i < nVertices; ++i)
      {
        data.push_back(textureCoords[i][0]);
        data.push_back(textureCoords[i][1]);
      }
    }
  }

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(F32), data.data(), GL_STATIC_DRAW);

  if (indices.size() > 0) // indices
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(UI32), indices.data(), GL_STATIC_DRAW);
  }

  if (interleave)
  {
    size_t stride = SIZE_OF_VEC3;
    if (normals.size() > 0)
    {
      stride += SIZE_OF_VEC3;
    }
    if (textureCoords.size() > 0)
    {
      stride += SIZE_OF_VEC2;
    }
    
    size_t offset = 0;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offset);
    glEnableVertexAttribArray(0);
    offset += SIZE_OF_VEC3;
    
    if (normals.size() > 0)
    {
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offset);
      glEnableVertexAttribArray(1);
      offset += SIZE_OF_VEC3;
    }

    if (textureCoords.size() > 0)
    {
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offset);
      glEnableVertexAttribArray(2);
    }
  }
  else
  {
    size_t offset = 0;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
    glEnableVertexAttribArray(0);
    offset += positions.size() * SIZE_OF_VEC3;

    if (normals.size() > 0)
    {
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
      glEnableVertexAttribArray(1);
      offset += normals.size() * SIZE_OF_VEC3;
    }

    if (textureCoords.size() > 0)
    {
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);
      glEnableVertexAttribArray(2);
    }
  }

  glBindVertexArray(0); // unbind
}

void Mesh::draw() const
{
  glBindVertexArray(VAO);
  EBO 
    ? glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0) 
    : glDrawArrays(GL_TRIANGLES, 0, (GLsizei)positions.size());
}

