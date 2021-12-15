/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#ifndef MESH_H
#define MESH_H

#include <render/primitives/Primitive.h>

#include <vector>

class Mesh : protected Primitive
{  
  friend class OBJLoader;

  friend class KDOP;

public:
  AABB getBounds();

  bool intersect(const Ray& ray);

  bool intersectP(const Ray& ray);

  /* OPENGL */
  void generatebuffers(bool interleave);

  void draw();

  void test() {}

protected:
  std::vector<Primitive*> triangles;

  // mesh data
  std::vector<UI32> indices;
  std::vector<Vector3> positions;
  std::vector<Vector3> normals;
  std::vector<Vector2> textureCoords;
};

#endif // ! MESH_H