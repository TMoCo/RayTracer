#ifndef MESH_H_
#define MESH_H_

#include <math/thomath.h>

#include <vector>

#include <glad/glad.h>

struct Mesh
{
  // gl buffer handles
  UI32 vao, vbo, ebo;

  ~Mesh()
  {
    if (vao)
      glDeleteVertexArrays(1, &vao);
    if (vbo)
      glDeleteBuffers(1, &vbo);
    if (ebo)
      glDeleteBuffers(1, &ebo);
  }
  
  // mesh data
  std::vector<Vector3> positions;
  std::vector<Vector3> normals;
  std::vector<Vector2> textureCoords;

  std::vector<UI32> indices;

  void generatebuffers(bool interleave);

  void draw();
};

#endif // ! MESH_H_