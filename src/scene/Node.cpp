#include <scene/Node.h>

void Node::updateTransform()
{
    dirty = false;
}

void Node::clear()
{
  for (auto& child : children)
  {
    child->clear();
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

Transform Node::getLocalTransform()
{
  return local;
}

Transform Node::getGlobalTransform()
{
  if (dirty)
    updateTransform();
  if (parent)
    global = parent->getGlobalTransform(); // multiply by local 
  else
    global = local;
  return global;
}