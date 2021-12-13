/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
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

  Primitive* primitive; // can be an aggregate (eg: a mesh), or simple geometry (eg: a sphere)
};

#endif // !NODE_H_
