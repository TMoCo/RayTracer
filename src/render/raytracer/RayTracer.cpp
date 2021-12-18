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
      // possible thread code:
      // while(!stack.empty())
      //   get pixel from stack (can't be accessed by other threads)
      //   compute pixel colour
      //   pop pixel 
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
     return 0.5f * castRay(Ray{ surfel.position, uniformSampleHemisphere(surfel.normal), INFINITY }, ++depth);
  }
  else
  {
    F32 t = 0.5f * (inRay.direction.normalize()[1] + 1.0f);
    return (1.0f - t) * colour { 1.0f, 1.0f, 1.0f } + t * colour{ 0.5f, 0.7f, 1.0f };
  }
    

    //PRINT("intersection");
    // compute surfel position, normal and texture coordinates

    // colour with uvs
    // Vector3 barycentric = UniformSampleTriangle(Random::UniformUV());

    // colour = surfel.UV()._v;
    // colour = barycentric._v;


    // direct lighting
    /*
    Surfel shadow;
    for (UI32 l = 0; l < lights.size(); ++l) {
      tNear = INFINITY;

      // from light to surfel
      ray.origin = RandomAreaLightPoint(lights[l]); // ray originate at light
      ray.direction = (surfel.position - ray.origin).normalize(); // to surfel

      // get intersection of light to surfel

      // > same mesh and same triangle = not in shadow
      //if ((shadow.mesh == surfel.mesh) && (shadow.tri == surfel.tri)) {
        // compute lighting
        c += {};
          /*
            // light colour
            model.materials.at(model.lights[l]->material).emissive.toVector3()
            // surfel brdf
            * surfel.BRDF(-ray.direction, -inRay.direction,
            model.materials.at(surfel.mesh->material))
            // light distance attenuation
            / (tNear * tNear);
          
      //}
    }

    // indirect lighting
    ray.direction = Quaternion::rotateVector(
      // random point on hemisphere unit hemisphere
      UniformSampleHemisphere(random::UniformUV()),
      // rotation to align unit normal with surfel normal
      Quaternion::getRotationFrom(UP, surfel.normal));

    c += CastRay(ray, depth + 1);
    */
    //* surfel.BRDF(ray.direction, -inRay.direction, model.materials.at(surfel.mesh->material));
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
    return uniformSampleTriangle(random::uniformUV());
    // random point in the triangle
    /*
    return 
        light->positions[index  ] * barycentric.x +
        light->positions[index+3] * barycentric.y +
        light->positions[index+3] * barycentric.z;
    */
}

Vector2 RayTracer::uniformSampleDisk(const Vector2& uv) const
{
  // https://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/2D_Sampling_with_Multidimensional_Transformations
    
  // to range [-1,1]
  Vector2 p = uv * 2.0f - Vector2{ 1.0f, 1.0f };
  if (p.x + p.y == 0) 
  {
    return Vector2{}; // degenerate case
  }

  // generate a random point on a unit disk
  F32 r, theta;
  if (std::abs(p.x) > std::abs(p.y)) 
  {
    r = p.x;
    // distorts grid to avoid collecting at origin
    theta = PI_FOUR * (p.y / p.x);
  }
  else 
  {
    r = p.y;
    theta = PI_TWO - PI_FOUR * (p.x / p.y);
  }

  return r * Vector2{ std::cos(theta), std::sin(theta) };
}

Vector3 RayTracer::uniformSampleHemisphere (const Vector2& uv) const
{
  // generate a random vector on the unit hemisphere
  F32 r = std::sqrt(std::max(0.0f, 1.0f - uv.x * uv.x));
  F32 phi = TWO_PI * uv.y;
  return { r * std::cos(phi), r * std::sin(phi), uv.x };
}

Vector3 RayTracer::uniformSampleHemisphere(const Vector3& normal) const
{
  Vector3 inUnitSphere = uniformSampleUnitSphere();
  return inUnitSphere.dot(normal) > 0.0f ? inUnitSphere : -inUnitSphere;
}

Vector3 RayTracer::uniformSampleHemisphere(const Vector2& uv, const Vector3& normal) const
{
  return Quaternion::rotateVector(
    uniformSampleHemisphere(random::uniformUV()), Quaternion::getRotationFrom(UP, normal));
}

Vector3 RayTracer::uniformSampleUnitSphere() const
{
  Vector3 vector;
  while (true)
  {
    vector = random::uniformVector3(-1.0f, 1.0f);
    if (vector.lengthSquared() >= 1.0f)
    {
      continue;
    }
    return vector;
  }
}

Vector3 RayTracer::cosineSampleHemisphere(const Vector2& uv) const
{
  Vector2 d = uniformSampleDisk(uv);
  F32 z = std::sqrt(std::max(0.0f, 1.0f - d.x * d.x - d.y * d.y));
  return { d.x, d.y, z };
}

Vector3 RayTracer::uniformSampleTriangleBasuOwen(const F32& u) const
{
    // u in range [0,1], 1 << 32 = 2^32 (so must be UI64)
  UI32 uFixedPoint = static_cast<UI32>(u * (1ULL << 32));

  Vector2 A{1.0f, 0.0f}, B{0.0f, 1.0f}, C{0.0f, 0.0f};
  Vector2 An, Bn, Cn; // used in iteration
  UI32 digit; // base 4 digit
  for (UI32 i = 0; i < 16; ++i) 
  {
    // extract base4 digit
    digit = (uFixedPoint >> (2 * (15 - i))) & 3; // == (u / (15 - i)) % 4
    switch (digit) 
    {
    case 0:
      An = (B + C) * 0.5f;
      Bn = (A + C) * 0.5f;
      Cn = (A + B) * 0.5f;
      break;
    case 1:
      An = A;
      Bn = (A + B) * 0.5f;
      Cn = (A + C) * 0.5f;
      break;
    case 2:
      An = (B + A) * 0.5f;
      Bn = B;
      Cn = (B + C) * 0.5f;
      break;
    case 3:
      An = (C + A) * 0.5f;
      Bn = (C + B) * 0.5f;
      Cn = C;
      break;
    }
    A = An;
    B = Bn;
    C = Cn;
  }
  An = (A + B + C) / 3.0f;
  // barycentric coordinates
  return { An.x, An.y, 1.0f - An.x - An.y };
}

Vector3 RayTracer::uniformSampleTriangle(const Vector2& uv) const
{
  F32 su0 = std::sqrt(uv.x);
  F32 b0 = 1.0f - su0;
  F32 b1 = uv.y - su0;
  return { b0, b1, 1.0f - b0 - b1 };
}
