/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

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
    delete root;
  }
}

const std::vector<Primitive*>& Scene::getPrimitives() const 
{
  return primitives;
}

void Scene::draw() const
{
  /*
  for (auto primitive : primitives)
  {
    primitive->draw();
  }
  */
}