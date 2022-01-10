/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <core/debug.h>
#include <render/bounds/AABB.h>
#include <render/shapes/Triangle.h>
#include <render/primitives/Mesh.h>
#include <scene/Node.h>

Mesh::Mesh()
  : Primitive(), VAO{ 0 }, VBO{ 0 }, EBO{ 0 }, onGpu{ false }
{ }

Mesh::~Mesh()
{
  //glDeleteBuffers(1, &VBO);
  //glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);
}

const AABB* Mesh::getBounds()
{
  if (!bounds)
  {
    bounds = new AABB{};
    for (auto& p : pos)
    {
      bounds->mergeWithPoint(parent->getWorldTransform()->applyToPoint(p));
    }
  }

  return bounds;
}

bool Mesh::intersect(const rt::Ray& ray, Surfel* surfel) const 
{
  for (Triangle* triangle : triangles)
  {
    triangle->intersect(ray, surfel);
  }
  return ray.tMax < INFINITY;
}

void Mesh::draw(Shader* shader) const
{
  shader->setMatrix4("M", parent->getWorldTransform()->getMatrix());
  if (onGpu)
  {
    glBindVertexArray(VAO);
    EBO
      ? glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0)
      : glDrawArrays(GL_TRIANGLES, 0, (GLsizei)pos.size());
  }
}

void Mesh::generateBuffers(bool interleave)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  std::vector<float> data; // prepare mesh data to send to GPU
  size_t nVertices = pos.size(); // assumes at least positions
  data.reserve(nVertices * 3);
  if (nor.size() > 0)
  {
    data.reserve(data.capacity() + nVertices * 3);
  }
  if (tex.size() > 0)
  {
    data.reserve(data.capacity() + nVertices * 2);
  }

  if (interleave)
  {
    // todo: find better way to interleave data
    for (uint32_t i = 0; i < nVertices; ++i)
    {
      data.push_back(pos[i][0]);
      data.push_back(pos[i][1]);
      data.push_back(pos[i][2]);

      if (nor.size() > 0)
      {
        data.push_back(nor[i][0]);
        data.push_back(nor[i][1]);
        data.push_back(nor[i][2]);
      }

      if (tex.size() > 0)
      {
        data.push_back(tex[i][0]);
        data.push_back(tex[i][1]);
      }
    }
  }
  else
  {
    for (uint32_t i = 0; i < nVertices; ++i)
    {
      data.push_back(pos[i][0]);
      data.push_back(pos[i][1]);
      data.push_back(pos[i][2]);
    }

    if (nor.size() > 0)
    {
      for (uint32_t i = 0; i < nVertices; ++i)
      {
        data.push_back(nor[i][0]);
        data.push_back(nor[i][1]);
        data.push_back(nor[i][2]);
      }
    }

    if (tex.size() > 0)
    {
      for (uint32_t i = 0; i < nVertices; ++i)
      {
        data.push_back(tex[i][0]);
        data.push_back(tex[i][1]);
      }
    }
  }

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

  if (indices.size() > 0) // indices
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
  }

  if (interleave)
  {
    size_t stride = SIZE_OF_VEC3;
    if (nor.size() > 0)
    {
      stride += SIZE_OF_VEC3;
    }
    if (tex.size() > 0)
    {
      stride += SIZE_OF_VEC2;
    }
    
    size_t offset = 0;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offset);
    glEnableVertexAttribArray(0);
    offset += SIZE_OF_VEC3;
    
    if (nor.size() > 0)
    {
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offset);
      glEnableVertexAttribArray(1);
      offset += SIZE_OF_VEC3;
    }

    if (tex.size() > 0)
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
    offset += pos.size() * SIZE_OF_VEC3;

    if (nor.size() > 0)
    {
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
      glEnableVertexAttribArray(1);
      offset += nor.size() * SIZE_OF_VEC3;
    }

    if (tex.size() > 0)
    {
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);
      glEnableVertexAttribArray(2);
    }
  }
  onGpu = true;
  glBindVertexArray(0); // unbind
}

void Mesh::generateTriangles()
{
  size_t numTriangles = indices.size() / 3;
  
  if (numTriangles == 0)
  {
    DEBUG_PRINT("Could not generate triangles (no indices)!");
    return;
  }

  triangles.reserve(numTriangles);

  for (uint32_t i = 0; i < indices.size(); i+=3)
  {
    triangles.push_back(new Triangle(parent->getWorldTransform(), this, &indices[i]));    
  }
}

void Mesh::generateNormals()
{
  nor.resize(pos.size());
  if (indices.size() > 0)
  {
    for (size_t i = 0; i < indices.size(); i+=3 )
    {
      nor[i / 3] = (pos[indices[i]] - pos[indices[i + 1]]).cross(pos[indices[i]] - pos[indices[i + 2]]).normalize();
    }
  }
  else
  {
    for (size_t i = 0; i < pos.size(); i += 3)
    {
      nor[i] = (pos[i] - pos[i + 1]).cross(pos[i] - pos[i + 2]).normalize();
    }
  }
}

void Mesh::generateTangents()
{
  // todo
}