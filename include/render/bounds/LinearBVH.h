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

    ShapePrimitiveInfo(int num, const AABB& bounds)
      : num{ num }, bounds{ bounds }, centroid{ bounds.getCentroid() }
    { }

    int num;
    AABB bounds;
    Vector3 centroid;
  };

  struct MortonPrimitive
  {
    int shapeIndex;
    uint32_t mortonCode;
  };

  struct BVHNode
  {
    void makeLeafNode(int offset, int n, const AABB& b)
    {
      shapeOffset = offset;
      nShapes = n;
      bounds = b;
      children[0] = children[1] = nullptr;
    }

    void makeInteriorNode(int axis, BVHNode* firstChild, BVHNode* secondChild)
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
    int axisSplit, shapeOffset, nShapes; // index into ordered shapes vector
  };

  struct LBVHTreelet
  {
    int startIndex, nShapes;
    BVHNode* nodes;
  };

  struct LinearBVHNode
  {
    AABB bounds;
    union // interpret int differently if leaf or interior node
    {
      int shapeOffset;
      int secondChildOffset;
    };
    int16_t nShapes; // 0 for interior nodes
    int8_t axis; 
    int8_t pad[1]; // sizeof(LinearBVHNode) -> 32
  };

public:
  LinearBVH();

  ~LinearBVH();

  LinearBVH(std::vector<Shape*> sceneShapes);

  void draw(Shader* shader) const;

  bool intersect(const Ray& ray, Surfel* surfel);

  void getGlData();

private:
  BVHNode* buildTree(MemoryArena& arena, std::vector<ShapePrimitiveInfo>& shapeInfo, int start, int end, 
    int* totalNodes, std::vector<Shape*>& orderedShapes);

  BVHNode* buildHorizontalLinearBVH(MemoryArena& arena, const std::vector<ShapePrimitiveInfo>& shapeInfo,
    int* totalNodes, std::vector<Shape*>& orderedShapes);

  uint32_t apply3DMortonEncoding(const Vector3& point);

  uint32_t shift10BitsLeftToEvery3rd(uint32_t x);

  void radixSortMortonPrimitives(std::vector<MortonPrimitive>* mortonPrimitives);

  BVHNode* buildTreelet(BVHNode*& buildNodes, const std::vector<ShapePrimitiveInfo>& primitiveInfo,
    MortonPrimitive* mortonPrims, int nShapes, int* totalNodes, std::vector<Shape*>& orderedShapes,
    int* orderedShapeOffset, int bitIndex);

  BVHNode* buildUpperSAH(MemoryArena& arena, std::vector<BVHNode*>& treeletRoots, int start, int end, int* totalNodes);

  int flattenBVHTree(BVHNode* node, int* offset);

private:
  uint32_t VAO, VBO, IBO;

  LinearBVHNode* linearBVH;

  int totalNodes;

  std::vector<Shape*> sceneShapes;
};


#endif // !LINEAR_BVH_H
