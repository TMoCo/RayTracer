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


#include <render/primitives/Primitive.h>

#include <vector>

class AABB;
class Scene;
class Primitive;

class BVH : public Primitive
{
public:
  BVH();

  BVH(const Scene* scene);

  ~BVH();

  virtual const AABB* getBounds() override;

  virtual bool intersect(const Ray& ray, Surfel* surfel) const override;

  virtual void draw(Shader* shader) const override;

private:

  uint32_t VAO, VBO, IBO;

  std::vector<Matrix4> AABBTransforms;

  const Scene* scene;
};

#endif // !BVH_H