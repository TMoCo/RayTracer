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

#include <render/Camera.h>

#include <scene/Node.h>

class Scene 
{
  friend class SceneLoader;

public:
  std::string name;

  Scene();

  ~Scene();
  
  void clear(); // dfs delete nodes

  void draw() const;

  const std::vector<Primitive*>& getPrimitives() const;

  Node* root;
private:

  std::vector<Primitive*> primitives; // container for all primitives
};

#endif // !SCENE_H
