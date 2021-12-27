/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 20/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Bounding volume hierarchy class declaration using AABB
//

#ifndef BVH_H
#define BVH_H 1

#include <core/types.h>

#include <vector>

class AABB;
class Scene;
class Primitive;

class BVH
{
public:
  BVH();

  BVH(const Scene* scene);

  ~BVH();

  void draw() const;

private:
  void generateOpenGLData();

  UI32 VAO, VBO, IBO;

  const std::vector<Primitive*>* scenePrimitives;

  std::vector<Matrix4> AABBTransforms;
};

#endif // !BVH_H