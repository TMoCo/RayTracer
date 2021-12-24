/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 22/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#ifndef MESH_H
#define MESH_H 1

#include <render/primitives/Primitive.h>

#include <vector>

class Mesh : public Primitive
{  
  friend class OBJLoader;
  friend class SceneLoader;
  friend class Triangle;

public:
  Mesh();

  AABB getBounds() const override;

  bool intersect(const Ray& ray, Surfel* surfel) const override;

  void test() override;

  void draw() const;

protected:
  void generateBuffers(bool interleave);

  void generateTriangles();

  void generateNormals();

  void generateTangents(); // todo: add tangents for normal mapping

protected:
  UI32 VAO, VBO, EBO;

  std::vector<Triangle*> triangles;

  std::vector<UI32> indices;
  std::vector<Vector3> pos;
  std::vector<Vector3> nor;
  std::vector<Vector2> tex;


  bool onGpu;
};

#endif // ! MESH_H