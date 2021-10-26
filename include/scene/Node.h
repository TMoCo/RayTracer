//
// A class for managing a scene
//

#ifndef NODE_H_
#define NODE_H_ 1

#include <vector>

#include <math/Transform.h>

struct Node
{
  std::string name;

  Node* parent;
  std::vector<Node*> children;

  Transform local;
  Transform global;

  bool dirty;

  void updateTransform();

  void clear();

  void addChild(Node* node);

  Node* getChild(const std::string& name);

  std::vector<Node*>& getChildren();
  
  void setParent(Node* newParent);
  
  Node* getParent();

  Transform getLocalTransform();
  
  Transform getGlobalTransform();
};

#endif // !NODE_H_
