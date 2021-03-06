/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <scene/Scene.h>

Scene::Scene()
  : root(nullptr), linearBVH{ nullptr }
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

  if (linearBVH) 
  {
    delete linearBVH;
  }
}

void Scene::draw(Shader* shader) const
{ 
  for (uint32_t node = 0; node < root->children.size(); node++)
  {
    root->children[node]->draw(shader); // special case no calling root->draw as root has no primitives associated
  }
}

LinearBVH* Scene::buildLinearBVH()
{
  linearBVH = new LinearBVH{ shapes };
  return linearBVH;
}

bool Scene::intersectLBVH(const rt::Ray& inRay, Surfel* surfel) const
{
  return linearBVH->intersect(inRay, surfel);
}

bool Scene::intersectPrimitives(const rt::Ray& inRay, Surfel* surfel) const
{
  for (auto& primitive : primitives)
  {
    primitive->intersect(inRay, surfel);
  }
  return inRay.tMax < INFINITY;
}