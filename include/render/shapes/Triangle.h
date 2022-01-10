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
  Triangle(Transform* toWorld, const Mesh* mesh, const uint32_t* index)
    : Shape(toWorld), mesh{ mesh }, index{ index }
  { }

  AABB getAABB() const override
  {
    AABB bounds{};
    bounds.mergeWithPoint(toWorld->applyToPoint(mesh->pos[*index]));
    bounds.mergeWithPoint(toWorld->applyToPoint(mesh->pos[*(index + 1)]));
    bounds.mergeWithPoint(toWorld->applyToPoint(mesh->pos[*(index + 2)]));
    return bounds;
  }

  bool intersect(const rt::Ray& ray, Surfel* surfel) const override
  {
    rt::Ray toObjRay = toWorld->applyInverseToRay(ray);
    // TODO: perform moller trumbore algorithm here
    Vector3 edge1, edge2, h, s, q;
    float k;

    edge1 = mesh->pos[*(index + 1)] - mesh->pos[*index];
    edge2 = mesh->pos[*(index + 2)] - mesh->pos[*index];

    h = toObjRay.direction.cross(edge2);
    k = edge1.dot(h);

    // if cross prod of ray and edge2 is perpendicular to egde1, then 
    // ray is parallel to triangle
    if (k > -EPSILON && k < EPSILON)
    {
      return false;
    }

    k = 1.0f / k; // reuse k

    // s = origin - v0
    s = toObjRay.origin - mesh->pos[*index];
    q = s.cross(edge1);

    float a, b;

    a = k * s.dot(h);
    b = k * toObjRay.direction.dot(q); // barycentric uv
    if (b < 0.0f || a < 0.0f || a + b > 1.0f)
    {
      return false;
    }

    float t = k * edge2.dot(q);
    if (t > EPSILON && t < ray.tMax)
    {
      Vector3 pHit = ray.at(t);
      Vector3 normal = mesh->nor[*index] * a + mesh->nor[*(index + 1)] * b + mesh->nor[*(index + 2)] * (1.0f - a - b);
      Vector2 uv = mesh->tex[*index] * a + mesh->tex[*(index + 1)] * b + mesh->tex[*(index + 2)] * (1.0f - a - b);
      *surfel = { pHit, normal, uv, -ray.direction, this };
      surfel->material = mesh->material;
      ray.tMax = t;
      return true;
    }
    return false;
  }

  float getArea() const override
  {
    const Vector3& p0 = mesh->pos[*index];
    const Vector3& p1 = mesh->pos[*(index + 1)];
    const Vector3& p2 = mesh->pos[*(index + 2)];
    return 0.5f * (p1 - p0).cross(p2 - p0).length();
  }

private:
  const Mesh* mesh;

  const uint32_t* index;
};

#endif // !TRIANGLE_H
