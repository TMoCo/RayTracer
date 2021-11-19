//
// A class for managing a scene
//

#ifndef NODE_H_
#define NODE_H_ 1

#include <scene/Transform.h>

#include <vector>

class Node
{
  friend class SceneLoader;
public:
  std::string name;

  Node();
  
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
};

#endif // !NODE_H_
