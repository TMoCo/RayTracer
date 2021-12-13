/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// type of primitive for a single geometric shape
//

#ifndef TRIANGLE_MESH_H_
#define TRIANGLE_MESH_H_ 1

#include <render/raytracer/Primitive.h>
#include <render/shapes/Shape.h>

class TriangleMesh : protected Primitive
{
  friend class SceneLoader;
public:
  TriangleMesh() : Primitive() {}

  // these should never be called
  AABB getBounds() {}
  bool intersect(const Ray& ray) {}
  bool intersectP(const Ray& ray) {}

  // for computing lighting 
  // virtual const AreaLight* getAreaLight() = 0;

  // virtual const Material* getMaterial() = 0;

protected:
  std::vector<Primitive*> triangles;
  // Material* material;
};

#endif // !TRIANGLE_MESH_H_
