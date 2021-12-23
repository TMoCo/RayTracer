/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <scene/Node.h>

Node::Node(const std::string& name, Node* parent) 
  : name(name), parent(parent), primitive(nullptr), outOfDate{ true }
{ }

Node::~Node()
{
  clear();
}

Transform* Node::getLocalTransform()
{
  return &local;
}

Transform* Node::getWorldTransform()
{
  if (outOfDate)
  {
    world = local;
    Node* parentNode = parent;
    while (parentNode != nullptr)
    {
      world = parentNode->local.applyToTransform(world);
      parentNode = parent->parent;
    }
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

const Primitive* Node::setPrimitive(Primitive* p)
{
  if (p->parent)
  {
    p->parent->primitive = nullptr; // relieve primitive's previous parent
  }

  p->parent = this;
  primitive = p;

  return primitive;
}

void Node::clear()
{
  for (auto& child : children)
  {
    child->clear(); 
    delete child;
  }
}
