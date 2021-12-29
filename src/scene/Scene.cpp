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

void Scene::draw() const
{ }

BVH* Scene::buildBVH()
{
  if (bvh)
  {
    delete bvh;
  }

  bvh = new BVH{ this };
  return bvh;
}

bool Scene::intersect(const Ray& inRay, Surfel* surfel) const
{
  // if bvh on:
  return bvh->intersect(inRay, surfel);

  for (auto& primitive : primitives)
  {
    primitive->intersect(inRay, surfel);
  }

  return inRay.tMax < INFINITY;
}