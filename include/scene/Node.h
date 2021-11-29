//
// A class for managing a scene
//

#ifndef NODE_H_
#define NODE_H_ 1

#include <render/primitives/Primitive.h>
#include <scene/Transform.h>

#include <vector>

class Node
{
  friend class SceneLoader;
public:
  std::string name;

  Node(const std::string& name, Node* parent = nullptr);
  
  ~Node();

  void clear();

  void addChild(Node* node);

  Node* getChild(const std::string& name);

  std::vector<Node*>& getChildren();
  
  void setParent(Node* newParent);
  
  Node* getParent();

  void updateTransform();

protected:
  Node* parent;

  std::vector<Node*> children;

  Transform local;

  Transform global;

  bool dirty;

  Primitive* primitive; // can be an aggregate, geometry, a mesh
};

#endif // !NODE_H_
