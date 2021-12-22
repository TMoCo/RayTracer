/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 22/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Triangle shape class declaration
//

#ifndef TRIANGLE_H
#define TRIANGLE_H 1

#include <render/shapes/Shape.h>
#include <render/Primitives/Mesh.h>
#include <render/raytracer/Surfel.h>

class Triangle : public Shape
{
public:
  Triangle(Transform* toWorld, const Mesh* mesh, UI32 index)
    : Shape(toWorld), mesh{ mesh }, index{ index }
  { }

  AABB getAABB() const override
  {
    return {};
  }

  bool intersect(const Ray& ray, F32* tHit, Surfel* surfel) const override
  {
    // perform moller trumbore algorithm here
    return false;
  }

  F32 getArea() const override
  {
    const Vector3& p0 = mesh->positions[index];
    const Vector3& p1 = mesh->positions[(size_t)index + 1];
    const Vector3& p2 = mesh->positions[(size_t)index + 2];
    return 0.5f * (p1 - p0).cross(p2 - p0).length();
  }

private:
  const Mesh* mesh;

  UI32 index;
};

#endif // !TRIANGLE_H
