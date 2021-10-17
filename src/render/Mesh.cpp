#include <render/Mesh.h>

#include <core/debug.h>

#include <glad/glad.h>

void Mesh::generateBuffers(bool interleave)
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  // for now assert that models have positions, normals and uvs
  DEBUG_ASSERT(positions.size() == normals.size() && positions.size() == textureCoords.size(), "");
  std::vector<F32> data{};
  if (interleave)
  {
    for (UI32 i = 0; i < positions.size(); ++i)
    {
      data.push_back(positions[i].x);
      data.push_back(positions[i].y);
      data.push_back(positions[i].z);

      data.push_back(normals[i].x);
      data.push_back(normals[i].y);
      data.push_back(normals[i].z);

      data.push_back(textureCoords[i].x);
      data.push_back(textureCoords[i].y);
    }
  }
  else
  {
    for (UI32 i = 0; i < positions.size(); ++i)
    {
      data.push_back(positions[i].x);
      data.push_back(positions[i].y);
      data.push_back(positions[i].z);
    }

    for (UI32 i = 0; i < normals.size(); ++i)
    {
      data.push_back(normals[i].x);
      data.push_back(normals[i].y);
      data.push_back(normals[i].z);
    }

    for (UI32 i = 0; i < textureCoords.size(); ++i)
    {
      data.push_back(textureCoords[i].x);
      data.push_back(textureCoords[i].y);
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
    UI32 stride = 8 * sizeof(F32);
    UI32 offset = 0;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(0);
    offset += 3 * sizeof(F32);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(1);
    offset += 3 * sizeof(F32);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(2);
  }
  else
  {
    UI32 offset = 0;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, &offset);
    offset += positions.size() * sizeof(F32);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, &offset);
    offset += normals.size() * sizeof(F32);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, &offset);
  }

  glBindVertexArray(0); // unbind
}

void Mesh::draw()
{
  // bind vao and draw
  glBindVertexArray(vao);
  ebo 
    ? glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0) 
    : glDrawArrays(GL_TRIANGLES, 0, positions.size());
}