//
// Bounding volume hierarchy class
//

#ifndef BHV_H_
#define BVH_H_

#include <math/thomath.h>

#include <render/bounds/AABB.h>

struct BVH
{
  UI32 vao, vbo, ebo;

  struct Primitive
  {
    // primitive index
    UI32 index;
    // bounds + centre
    AABB bbox{};
  };

  void getPrimitives(const std::vector<Mesh*>& meshes); // from meshes
    
  void generateBuffers();
  void draw();

  std::vector<Primitive> primitives;
};

#endif