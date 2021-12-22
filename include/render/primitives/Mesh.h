/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 22/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#ifndef MESH_H
#define MESH_H 1

#include <render/primitives/Primitive.h>

#include <vector>

class Mesh : protected Primitive
{  
  friend class OBJLoader;

  friend class Triangle;

public:
  Mesh();

  AABB getBounds() const override;

  bool intersect(const Ray& ray, Surfel* surfel) const override;

  void test() override;

  const Material* getMaterial() const override;

  void draw() const;

  void generateBuffers(bool interleave);

protected:
  UI32 VAO, VBO, EBO;

  std::vector<Primitive*> triangles;

  std::vector<UI32> indices;
  std::vector<Vector3> positions;
  std::vector<Vector3> normals;
  std::vector<Vector2> textureCoords;
};

#endif // ! MESH_H