/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <scene/Scene.h>

Scene::Scene() 
  : root(nullptr)
{ }

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

void Scene::draw() const
{ }

const std::vector<Primitive*>* Scene::getPrimitives() const 
{
  return &primitives;
}

bool Scene::intersect(const Ray& inRay, Surfel* surfel) const
{
  for (auto& primitive : primitives)
  {
    primitive->intersect(inRay, surfel);
  }
  return inRay.tMax < INFINITY;
}