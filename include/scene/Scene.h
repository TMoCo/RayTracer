//
// A tree data structure for managing a scene
//

#ifndef SCENE_H_
#define SCENE_H_ 1

#include <render/Camera.h>

#include <scene/Node.h>
#include <scene/Primitive.h>

struct Scene {
  std::string name;

  Node* root;

  Scene();
  ~Scene();
  
  void clear(); // dfs delete nodes

  std::vector<Primitive*> primitives; // container for all primitives
};

#endif // !SCENE_H_
