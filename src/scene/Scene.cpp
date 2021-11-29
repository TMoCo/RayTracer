#include <scene/Scene.h>

Scene::Scene() : root(nullptr)
{

}

Scene::~Scene()
{
  clear();
}

void Scene::clear()
{
  if (root)
  {
    root->clear();
    delete root;
  }
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