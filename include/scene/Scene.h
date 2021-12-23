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

class Ray;
class Surfel;

class Scene 
{
  friend class SceneLoader;

public:
  std::string name;

  Scene();

  ~Scene();
  
  void clear(); // dfs delete nodes

  void draw() const;

  const std::vector<Primitive*>* getPrimitives() const;

  bool intersect(const Ray& inRay, Surfel* surfel) const;

private:
  Node* root;

  std::vector<Primitive*> primitives; // container for all primitives
};

#endif // !SCENE_H
