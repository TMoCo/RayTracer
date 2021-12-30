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
#include <render/shapes/Shape.h>

class LinearBVH
{
  struct ShapePrimitiveInfo
  {
    ShapePrimitiveInfo(UI32 num, const AABB& bounds)
      : num{ num }, bounds{ bounds }, centroid{ bounds.getCentroid() }
    { }

    UI32 num;
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
    void leafNode(I32 o, I32 n, const AABB& b)
    {
      offset = o;
      nShapes = n;
      bounds = b;
      children[0] = children[1] = nullptr;
    }

    void interiorNode(I32 axis, BVHNode* firstChild, BVHNode* secondChild)
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
    I32 axisSplit, offset, nShapes; // index into ordered shapes vector
  };

  struct LBVHTreelet
  {
    I32 startIndex, nShapes;
    BVHNode* nodes;
  };

public:
  LinearBVH();

  LinearBVH(const std::vector<Shape*>& sceneShapes);

private:
  UI32 apply3DMortonEncoding(const Vector3& point);

  UI32 shift10BitsLeftToEvery3rd(UI32 x);

  void radixSortMortonPrimitives(std::vector<MortonPrimitive>* mortonPrimitives);

  std::vector<Shape*> sceneShapes;
};


#endif // !LINEAR_BVH_H
