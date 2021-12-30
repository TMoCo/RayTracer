/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// A tree data structure for managing a scene
//

#ifndef SCENE_H
#define SCENE_H 1

#include <scene/Node.h>
#include <render/shapes/Shape.h>
#include <render/primitives/BVH.h>

class Ray;
class Surfel;

class Scene 
{
  friend class SceneLoader;
  friend class BVH;

public:
  std::string name;

  Scene();

  ~Scene();
  
  void clear(); // dfs delete nodes

  void draw(Shader* shader) const;

  BVH* buildBVH();

  bool intersect(const Ray& inRay, Surfel* surfel) const;

  BVH* bvh;
protected:

  Node* root;

  std::vector<Primitive*> primitives; // container for all primitives

  std::vector<Shape*> shapes;
};

#endif // !SCENE_H
