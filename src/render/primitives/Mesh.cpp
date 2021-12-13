/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <render/primitives/Mesh.h>

#include <core/debug.h>

AABB Mesh::getBounds() 
{
  return AABB::getAABB(positions);
}

bool Mesh::intersect(const Ray& ray) 
{
  return false;
}

bool Mesh::intersectP(const Ray& ray) 
{
  return false;
}

void Mesh::generatebuffers(bool interleave)
{
  /*
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  // guaranteed to at least have vertex position data
  UI64 nVertices = positions.size();

  std::vector<F32> data; 
  data.reserve(nVertices * 3);
  
  // allocate space depending on extra mesh data
  if (normals.size() > 0)
    data.reserve(data.capacity() + nVertices * 3);
  if (textureCoords.size() > 0)
    data.reserve(data.capacity() + nVertices * 2);

  if (interleave)
  {
    // todo: find better way to interleave data
    for (UI32 i = 0; i < nVertices; ++i)
    {
      data.push_back(positions[i].x);
      data.push_back(positions[i].y);
      data.push_back(positions[i].z);

      if (normals.size() > 0)
      {
        data.push_back(normals[i].x);
        data.push_back(normals[i].y);
        data.push_back(normals[i].z);
      }

      if (textureCoords.size() > 0)
      {
        data.push_back(textureCoords[i].x);
        data.push_back(textureCoords[i].y);
      }
    }
  }
  else
  {
    for (UI32 i = 0; i < nVertices; ++i)
    {
      data.push_back(positions[i].x);
      data.push_back(positions[i].y);
      data.push_back(positions[i].z);
    }

    if (normals.size() > 0)
    {
      for (UI32 i = 0; i < nVertices; ++i)
      {
        data.push_back(normals[i].x);
        data.push_back(normals[i].y);
        data.push_back(normals[i].z);
      }
    }

    if (textureCoords.size() > 0)
    {
      for (UI32 i = 0; i < nVertices; ++i)
      {
        data.push_back(textureCoords[i].x);
        data.push_back(textureCoords[i].y);
      }
    }
  }

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(F32), data.data(), GL_STATIC_DRAW);

  if (indices.size() > 0)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(UI32), indices.data(), GL_STATIC_DRAW);
  }

  if (interleave)
  {
    UI64 stride = 3 * sizeof(F32);
    if (normals.size() > 0)
      stride += 3 * sizeof(F32);
    if (textureCoords.size() > 0)
      stride += 2 * sizeof(F32);
    
    UI64 offset = 0;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offset);
    glEnableVertexAttribArray(0);
    offset += 3 * sizeof(F32);
    
    if (normals.size() > 0)
    {
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offset);
      glEnableVertexAttribArray(1);
      offset += 3 * sizeof(F32);
    }

    if (textureCoords.size() > 0)
    {
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (GLsizei)stride, (void*)offset);
      glEnableVertexAttribArray(2);
    }
  }
  else
  {
    UI64 offset = 0;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
    glEnableVertexAttribArray(0);
    offset += positions.size() * 3 * sizeof(F32);

    if (normals.size() > 0)
    {
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
      glEnableVertexAttribArray(1);
      offset += normals.size() * 3 * sizeof(F32);
    }

    if (textureCoords.size() > 0)
    {
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);
      glEnableVertexAttribArray(2);
    }
  }

  glBindVertexArray(0); // unbind
  */
}

void Mesh::draw()
{
  /*
  // bind vao and draw
  glBindVertexArray(vao);
  ebo 
    ? glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0) 
    : glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(positions.size()));
  */
}

