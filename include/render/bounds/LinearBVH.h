/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 29/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Linear bounding volume hierachy taken from:
// https://www.pbr-book.org/3ed-2018/Primitives_and_Intersection_Acceleration/Bounding_Volume_Hierarchies
// uses morton codes of the scene shapes' AABB's centroid to construct treelets, which form a larger tree structure
//

#ifndef LINEAR_BVH_H
#define LINEAR_BVH_H 1

#include <core/types.h>

#include <render/bounds/AABB.h>
#include <render/raytracer/Surfel.h>
#include <render/Shader.h>
#include <render/shapes/Shape.h>
#include <resource/MemoryArena.h>

#define MAX_SHAPES_PER_NODE 20

class LinearBVH
{
  struct ShapePrimitiveInfo
  {
    ShapePrimitiveInfo()
      : num{ 0 }
    { }

    ShapePrimitiveInfo(I32 num, const AABB& bounds)
      : num{ num }, bounds{ bounds }, centroid{ bounds.getCentroid() }
    { }

    I32 num;
    AABB bounds;
    Vector3 centroid;
  };

  struct MortonPrimitive
  {
    I32 shapeIndex;
    UI32 mortonCode;
  };

  struct BVHNode
  {
    void makeLeafNode(I32 offset, I32 n, const AABB& b)
    {
      shapeOffset = offset;
      nShapes = n;
      bounds = b;
      children[0] = children[1] = nullptr;
    }

    void makeInteriorNode(I32 axis, BVHNode* firstChild, BVHNode* secondChild)
    {
      children[0] = firstChild;
      children[1] = secondChild;
      bounds = firstChild->bounds;
      bounds.mergeWithAABB(secondChild->bounds);
      axisSplit = axis;
      nShapes = 0;
    }

    AABB bounds;
    BVHNode* children[2];
    I32 axisSplit, shapeOffset, nShapes; // index into ordered shapes vector
  };

  struct LBVHTreelet
  {
    I32 startIndex, nShapes;
    BVHNode* nodes;
  };

  struct LinearBVHNode
  {
    AABB bounds;
    union // interpret int differently if leaf or interior node
    {
      I32 shapeOffset;
      I32 secondChildOffset;
    };
    I16 nShapes; // 0 for interior nodes
    I8 axis; 
    I8 pad[1]; // sizeof(LinearBVHNode) -> 32
  };

public:
  LinearBVH();

  ~LinearBVH();

  LinearBVH(std::vector<Shape*> sceneShapes);

  void draw(Shader* shader) const;

  bool intersect(const Ray& ray, Surfel* surfel);

private:
  BVHNode* buildTree(MemoryArena& arena, std::vector<ShapePrimitiveInfo>& shapeInfo, I32 start, I32 end, 
    I32* totalNodes, std::vector<Shape*>& orderedShapes);

  BVHNode* buildHorizontalLinearBVH(MemoryArena& arena, const std::vector<ShapePrimitiveInfo>& shapeInfo,
    I32* totalNodes, std::vector<Shape*>& orderedShapes);

  UI32 apply3DMortonEncoding(const Vector3& point);

  UI32 shift10BitsLeftToEvery3rd(UI32 x);

  void radixSortMortonPrimitives(std::vector<MortonPrimitive>* mortonPrimitives);

  BVHNode* buildTreelet(BVHNode*& buildNodes, const std::vector<ShapePrimitiveInfo>& primitiveInfo,
    MortonPrimitive* mortonPrims, I32 nShapes, I32* totalNodes, std::vector<Shape*>& orderedShapes,
    I32* orderedShapeOffset, I32 bitIndex);

  BVHNode* buildUpperSAH(MemoryArena& arena, std::vector<BVHNode*>& treeletRoots, I32 start, I32 end, I32* totalNodes);

  I32 flattenBVHTree(BVHNode* node, I32* offset);

  void getGlData();

private:
  UI32 VAO, VBO, IBO;

  LinearBVHNode* linearBVH;

  I32 totalNodes;

  std::vector<Shape*> sceneShapes;
};


#endif // !LINEAR_BVH_H
