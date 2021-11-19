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

void Scene::draw()
{
  /*
  for (auto primitive : primitives)
  {
    primitive->draw();
  }
  */
}