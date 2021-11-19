//
// A tree data structure for managing a scene
//

#ifndef SCENE_H_
#define SCENE_H_ 1

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

  void draw();

private:
  Node* root;

  // std::vector<Primitive*> primitives; // container for all primitives
};

#endif // !SCENE_H_
