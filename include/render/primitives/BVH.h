//
// Bounding volume hierarchy class
//

#ifndef BHV_H_
#define BVH_H_ 1

#include <math/thomath.h>

#include <render/bounds/AABB.h>

struct BVHNode
{
  inline void initLeaf(UI32 first, UI32 num, const AABB& b)
  {
    firstPrim = first;
    numPrim = num;
    bbox = b;
    children[0] = children[1] = nullptr;
  }

  inline void initInternal(AXIS axis, BVHNode* left, BVHNode* right)
  {
    children[0] = left;
    children[1] = right;
    bbox = AABB::mergeAABB(left->bbox, right->bbox);
    splitAxis = axis;
    numPrim = 0;
  }

  AABB bbox{};
  UI32 firstPrim;
  UI32 numPrim;
  AXIS splitAxis;
  BVHNode* children[2];
};

class BVH
{
  void buildBVH(const std::vector<Mesh*>& meshes);

  BVHNode* buildNode(UI32 start, UI32 end);

  void getPrimitives(const std::vector<Mesh*>& meshes);
  
  void generatebuffers();
  
  void draw();

private:
  UI32 vao, vbo, ebo;

  UI64 numNodes;
  UI32 numPrimitives;
};

#endif