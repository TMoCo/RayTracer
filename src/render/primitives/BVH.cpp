/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <core/debug.h>
#include <render/primitives/BVH.h>
#include <render/bounds/AABB.h>
#include <scene/Scene.h>
#include <render/shapes/GLShapes.h>

BVH::BVH()
  : Primitive(), VAO{ 0 }, VBO{ 0 }, IBO{ 0 }, scene{ nullptr }
{ }

BVH::BVH(const Scene* scene)
  : Primitive(), VAO{ 0 }, VBO{ 0 }, IBO{ 0 }, scene{ scene }
{
  parent = scene->root;

  // get AABBs
  AABBTransforms.reserve(scene->primitives.size());
  for (auto& primitive : scene->primitives)
  {
    const AABB* bounds = primitive->getBounds();
    Vector3 position = bounds->getCentroid();
    Vector3 scale = bounds->getDiagonal() * 0.5f;
    AABBTransforms.push_back(Matrix4::translation(position) * Matrix4::scale(scale));
  }

  // instance buffer
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ARRAY_BUFFER, IBO);
  glBufferData(GL_ARRAY_BUFFER, SIZE_OF_MAT4 * scene->primitives.size(), AABBTransforms.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glGetError();

  // vertex buffer
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLCube::unitCube), GLCube::unitCube, GL_STATIC_DRAW); // TODO: may cause errors

  // vertex attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SIZE_OF_VEC3, (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, IBO);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, SIZE_OF_MAT4, (void*)0);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, SIZE_OF_MAT4, (void*)SIZE_OF_VEC4);
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, SIZE_OF_MAT4, (void*)(2 * SIZE_OF_VEC4));
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, SIZE_OF_MAT4, (void*)(3 * SIZE_OF_VEC4));

  glVertexAttribDivisor(1, 1);
  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);

  glBindVertexArray(0);
}

BVH::~BVH()
{
  //glDeleteBuffers(1, &VBO);
  //glDeleteBuffers(1, &IBO);
  if (VAO)
  {
    glDeleteVertexArrays(1, &VAO);
  }
}

const AABB* BVH::getBounds()
{
  if (!bounds)
  {
    bounds = new AABB{};
    for (auto& p : scene->primitives)
    {
      bounds->mergeWithAABB(*p->getBounds());
    }
  }

  return bounds;
}

bool BVH::intersect(const Ray& inRay, Surfel* surfel) const
{
  for (auto& primitive : scene->primitives)
  {
    if (primitive->bounds->intersect(inRay)) // bounds guaranteed != nullptr in constructor
    {
      primitive->intersect(inRay, surfel);
    }
  }
  return inRay.tMax < INFINITY;
}

void BVH::draw(Shader* shader) const
{
  glBindVertexArray(VAO);
  glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, GLCube::indices, (GLsizei)scene->primitives.size());
}