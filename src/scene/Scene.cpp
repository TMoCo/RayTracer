/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <scene/Scene.h>

Scene::Scene()
  : root(nullptr), bvh{ nullptr }
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

  if (bvh) 
  {
    delete bvh;
  }
}

void Scene::draw(Shader* shader) const
{ 
  for (UI32 node = 0; node < root->children.size(); node++)
  {
    root->children[node]->draw(shader); // special case no calling root->draw as root has no primitives associated
  }
}

BVH* Scene::buildBVH()
{
  if (bvh)
  {
    delete bvh;
  }

  bvh = new BVH{ this };

  linearBVH = new LinearBVH{ shapes };
  return bvh;
}

bool Scene::intersect(const Ray& inRay, Surfel* surfel) const
{
  // if bvh on:

  return linearBVH->intersect(inRay, surfel);

  return bvh->intersect(inRay, surfel);

  for (auto& primitive : primitives)
  {
    primitive->intersect(inRay, surfel);
  }
  return inRay.tMax < INFINITY;


}