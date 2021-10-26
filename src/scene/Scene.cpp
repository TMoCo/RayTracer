#include <scene/Scene.h>

Scene::Scene()
{
  root = new Node;
}

Scene::~Scene()
{
  clear();
}

void Scene::clear()
{
  root->clear();
}