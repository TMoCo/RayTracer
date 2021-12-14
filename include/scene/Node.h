/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

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

  Node* getChild(const std::string& name);

  std::vector<Node*>& getChildren();
  
  Node* getParent();
  
  void setParent(Node* newParent);

  void updateTransform();

  void addChild(Node* node);

protected:
  Node* parent;

  std::vector<Node*> children;

  Transform local;

  Transform global;

  Primitive* primitive; // can be an aggregate (eg: a mesh), or simple geometry (eg: a sphere)
};

#endif // !NODE_H_
