/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <scene/Node.h>

Node::Node(const std::string& name, Node* parent) : name(name), parent(parent), primitive(nullptr), outOfDate{ false }
{

}

Node::~Node()
{
  clear();
}

Transform* Node::getLocalTransform()
{
  if (outOfDate)
  {
    local.updateMatrixRepresentations();
    outOfDate = false;
  }

  return &local;
}

Transform* Node::getWorldTransform()
{
  if (outOfDate)
  {
    world = *getLocalTransform();
  
    Node* parentNode = parent;
    while (parentNode != nullptr)
    {
      world.position = Quaternion::rotateVector(world.position, parentNode->local.rotation) * parentNode->local.scale;
      world.rotation *= parentNode->local.rotation;
      world.scale += parentNode->local.scale;
    }
    world.updateMatrixRepresentations();
  }

  return &world;
}

void Node::addChildNode(Node* node)
{
  children.push_back(node);
}

Node* Node::getChildNode(const std::string& name)
{
  return nullptr;
}

std::vector<Node*>& Node::getChildrenNodes()
{
  return children;
}

void Node::setChildrenOutOfDate()
{
  for (Node* child : children)
  {
    child->outOfDate = true;
    child->setChildrenOutOfDate();
  }
}

void Node::setParentNode(Node* newParent)
{
  parent = newParent;
}

Node* Node::getParentNode()
{
  return parent;
}

Node* Node::getRootNode()
{
  Node* root = this;
  while (root->parent != nullptr)
  {
    root = root->parent;
  }
  return root;
}

void Node::clear()
{
  for (auto& child : children)
  {
    child->clear(); 
    delete child;
  }
}

void Node::translateNode(const Vector3& translation)
{
  local.position += translation;
  outOfDate = true;
}

void Node::rotateNode(const Quaternion& rotation)
{
  local.rotation *= rotation;
  outOfDate = true;
}

void Node::scaleNode(const Vector3& scale)
{
  local.scale += scale;
  outOfDate = true;
}