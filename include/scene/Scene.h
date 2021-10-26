//
// A tree data structure for managing a scene
//

#ifndef SCENE_H_
#define SCENE_H_ 1

#include <render/Camera.h>

#include <scene/Node.h>

struct Scene {
  std::string name;

  Node* root;

  Scene();
  ~Scene();
  
  void clear(); // dfs delete nodes
};

#endif // !SCENE_H_
