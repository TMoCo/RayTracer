#ifndef MESH_H_
#define MESH_H_

#include <math/thomath.h>

#include <vector>

struct Mesh
{
  // gl buffer handles
  UI32 vao, vbo, ebo;

  ~Mesh();
  
  // mesh data
  std::vector<Vector3> positions;
  std::vector<Vector3> normals;
  std::vector<Vector2> textureCoords;

  std::vector<UI32> indices;

  void generateBuffers(bool interleave);

  void draw();
};

#endif // ! MESH_H_