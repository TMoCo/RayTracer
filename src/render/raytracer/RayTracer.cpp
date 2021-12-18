/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Raytracer class declaration
//

#include <core/core.h>
#include <core/random.h>
#include <render/raytracer/RayTracer.h>
#include <render/raytracer/UniformSampler.h>

void RayTracer::raytrace(buffer<colour>& frameBuffer, const Camera* camera, UI32 samples) const
{    
  // scale for aspect ratio
  UI32 width  = static_cast<UI32>(frameBuffer.getWidth()), height = static_cast<UI32>(frameBuffer.getHeight());
  F32 rWidth = 1.0f / static_cast<F32>(width), rHeight = 1.0f / static_cast<F32>(height);
  F32 scale = 1.0f / (F32)samples;

  std::cout << "Ray tracing...";
  // scan pixels from left to right, bottom to top, starting in bottom left corner
  auto start = sys_clock::now();
  for (UI32 row = 0; row < height; ++row)
  { 
    for (UI32 col = 0; col < width; ++col)
    {
      for (UI32 sample = 0; sample < samples; ++sample)
      {
        frameBuffer[row][col] +=
          castRay(Ray::generateCameraRay(camera, 
            { (col + random::uniformF32()) * rWidth, (row + random::uniformF32()) * rHeight }), 0);
      }
      frameBuffer[row][col] *= scale;
    }
  }
  auto end = sys_clock::now();
  std::cout << "Finished!\nTook: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}

void RayTracer::setScene(Scene* scene)
{
  this->scene = scene;
}

colour RayTracer::castRay(const Ray& inRay, UI32 depth) const
{
  colour c{};

  if (depth > MAX_DEPTH) // stop recursion 
  {
    return c;
  }

  Surfel surfel;
  
  for (auto& prim : scene->getPrimitives())
  {
    prim->intersect(inRay, &surfel);
  }

  if (inRay.tMax != INFINITY)
  {
     return 0.5f * castRay(Ray{ surfel.position, UniformSampler::hemisphere(surfel.normal), INFINITY }, ++depth);
  }
  else
  {
    F32 t = 0.5f * (inRay.direction.normalize()[1] + 1.0f);
    return (1.0f - t) * colour { 1.0f, 1.0f, 1.0f } + t * colour{ 0.5f, 0.7f, 1.0f };
  }
}

bool RayTracer::mollerTrumbore(const Ray& ray, const std::vector<Mesh*>& meshes, Surfel& surfel, F32& tNear) const
{
    // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    Vector3 edge1, edge2, h, s, q;
    F32 k, t, u, v;
    // test against all triangles in the model
    for (UI32 m = 0; m < meshes.size(); ++m) {
        // const Mesh& mesh = model.meshes[m];

        // apply bounding volume test here
        // if (!intersect(ray, mesh.bounds)) then skip mesh;
        /*
        for (UI32 f = 0; f < meshes[m]->indices.size(); f+=3) {
            edge1 = meshes[m]->positions[meshes[m]->indices[f+1]]
                - meshes[m]->positions[meshes[m]->indices[f]]; // V1 - v0
            edge2 = meshes[m]->positions[meshes[m]->indices[f+2]]
                - meshes[m]->positions[meshes[m]->indices[f]]; // V2 - v0

            h = ray.direction.cross(edge2);
            k = edge1.dot(h);

            // if cross prod of ray and edge2 is perpendicular to egde1, then 
            // ray is parallel to triangle
            if (k > -EPSILON && k < EPSILON) 
                continue;

            k = 1.0f / k; // reuse k

            // s = origin - v0
            s = ray.origin - meshes[m]->positions[meshes[m]->indices[f]]; 
            u = k * s.dot(h);
            if (u < 0.0f || u > 1.0f)
                continue;

            q = s.cross(edge1);
            v = k * ray.direction.dot(q);
            if (v < 0.0f || u + v > 1.0f)
                continue;

            k = k * edge2.dot(q);
            if (k > EPSILON) { // valid intersection
              return true;
                t = (ray.At(k) - ray.origin).length(); // test depth
                if (t < tNear) {
                    tNear = t;
                    surfel.tri = f;     // save index of current face
                    surfel.barycentric = {u, v, 1.0f - u - v}; // barycentric coordinates
                    surfel.mesh = meshes[m]; // ptr to mesh data
                }
            }
        }
        */
    }
    //return (surfel != nullptr); // no mesh == no intersection
    return false;
}

Vector3 RayTracer::randomAreaLightPoint(const Mesh* light) const
{
    // get a random triangle in the light mesh
    //UI32 index = Random::UniformI32(0, (static_cast<UI32>(light->indices.size()) / 9) - 1);
    // barycentric coordinates
  return {};
    // random point in the triangle
    /*
    return 
        light->positions[index  ] * barycentric.x +
        light->positions[index+3] * barycentric.y +
        light->positions[index+3] * barycentric.z;
    */
}

/*
Vector3 RayTracer::cosineSampleHemisphere(const Vector2& uv) const
{
  Vector2 d = uniformSampleDisk(uv);
  F32 z = std::sqrt(std::max(0.0f, 1.0f - d.x * d.x - d.y * d.y));
  return { d.x, d.y, z };
}
*/


