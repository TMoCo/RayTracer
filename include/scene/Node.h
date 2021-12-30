/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Scene node, building block of the scene data structure
//

#ifndef NODE_H
#define NODE_H 1

#include <render/primitives/Primitive.h>

#include <scene/Transform.h>

#include <vector>

class Node
{
  friend class SceneLoader;
  friend class Scene;

public:
  std::string name;

  Node(const std::string& name, Node* parent = nullptr);
  
  ~Node();

  Transform* getLocalTransform();

  Transform* getWorldTransform();

  Node* getChildNode(const std::string& name);

  std::vector<Node*>& getChildrenNodes();

  void setChildrenOutOfDate();
  
  Node* getParentNode();

  Node* getRootNode();
  
  void setParentNode(Node* newParent);

  void addChildNode(Node* node);

  const Primitive* setPrimitive(Primitive* p);

  void draw(Shader* shader) const;

  void clear();

protected:
  Node* parent;

  std::vector<Node*> children;

  Transform local;

  Transform world;

  Primitive* primitive; // can be an aggregate (eg: a mesh), or a shape (eg: a sphere)

  bool outOfDate;

};

#endif // !NODE_H
