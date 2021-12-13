/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <scene/Node.h>

Node::Node(const std::string& name, Node* parent) : name(name), parent(parent)
{

}

Node::~Node()
{
  clear();
}

void Node::updateTransform()
{

}

void Node::clear()
{
  for (auto& child : children)
  {
    child->clear(); // recursively delete all children
    delete child;
  }
}

void Node::addChild(Node* node)
{
  children.push_back(node);
}

Node* Node::getChild(const std::string& name)
{
  return nullptr;
}

std::vector<Node*>& Node::getChildren()
{
  return children;
}

void Node::setParent(Node* newParent)
{
  parent = newParent;
}

Node* Node::getParent()
{
  return parent;
}
