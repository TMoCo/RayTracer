//
// Raytracer class declaration
//

#include <raytracer/RayTracer.h>

#include <core/Random.h>

RayTracer::RayTracer() {}

void RayTracer::RayTraceImage(Image& buffer, Scene* scene, 
    Transform t, Camera* camera, UI32 samples) {
    
    // scale for aspect ratio
    F32 rWidth = 1.0f / (F32)buffer.Width(), rHeight = 1.0f / (F32)buffer.Height();

    Vector3 eye{0.0f, 0.0f, 0.0f}; // eye at origin

    // primary ray 
    Ray primaryRay;
    colour c;
    primaryRay._origin = eye;

    // gamma correction
    F32 gamma = 1.0f / 2.2f;

  // push pixels onto a stack


  for(UI32 row = 0; row < buffer.Height(); ++row) {
    for (UI32 col = 0; col < buffer.Width(); ++col) {
      //** Fit into a thread
         
      // thread code:
      // while(!stack.empty())
      //   get pixel from stack (can't be accessed by other threads)
      //   compute pixel colour
      //   pop pixel 

      c = {};

      // ray direction
      primaryRay = Ray::generateCameraRay(*camera, { (col + 0.5f) * rWidth, (row + 0.5f) * rHeight });
      // compute scene
      for (UI32 s = 0; s < samples; ++s)
          c += CastRay(primaryRay, 0);

      buffer[row][col] = Colour::gammaCorrection (Colour::reinhardExtendedTMO(c, 10.0f), gamma);

      //**
    }
  }
}

colour RayTracer::CastRay(const Ray& inRay, UI32 depth) {
    colour c{};

    if (depth > MAX_DEPTH) // stop recursion 
    {
        //PRINT("reached max depth");
        return c;
    }

    std::vector<Mesh*> meshes;
    std::vector<Mesh*> lights;

    // init variables
    Surfel surfel;
    F32 tNear = INFINITY;

    // intersection of ray with the model
    if (Intersect(inRay, meshes, surfel, tNear)) {
      //PRINT("intersection");
      // compute surfel position, normal and texture coordinates
      surfel.Interpolate();

      // colour with uvs
      // Vector3 barycentric = UniformSampleTriangle(Random::UniformUV());

      // colour = surfel.UV()._v;
      // colour = barycentric._v;

      Ray ray; // ray used for lighting

      // direct lighting
      Surfel shadow;
      for (UI32 l = 0; l < lights.size(); ++l) {
        tNear = INFINITY;

        // from light to surfel
        ray._origin = RandomAreaLightPoint(lights[l]); // ray originate at light
        ray._direction = (surfel.position - ray._origin).normalize(); // to surfel

        // get intersection of light to surfel
        Intersect(ray, meshes, shadow, tNear);

        // > same mesh and same triangle = not in shadow
        if ((shadow.mesh == surfel.mesh) && (shadow.tri == surfel.tri)) {
          // compute lighting
          c += {};
            /*
              // light colour
              model.materials.at(model.lights[l]->material).emissive.toVector3()
              // surfel brdf
              * surfel.BRDF(-ray._direction, -inRay._direction,
              model.materials.at(surfel.mesh->material))
              // light distance attenuation
              / (tNear * tNear);
            */
        }
      }

      // indirect lighting
      ray._direction = Quaternion::rotateVector(
        // random point on hemisphere unit hemisphere
        UniformSampleHemisphere(Random::UniformUV()),
        // rotation to align unit normal with surfel normal
        Quaternion::getRotationFrom(UP, surfel.normal));

      c += CastRay(ray, depth + 1);
      //* surfel.BRDF(ray._direction, -inRay._direction, model.materials.at(surfel.mesh->material));
    }
    else if (Intersect(inRay, lights, surfel, tNear))
      c = {};// &model.materials.at(surfel.mesh->material).emissive[0];

    return c;
}

bool RayTracer::Intersect(const Ray& ray, const std::vector<Mesh*>& meshes, 
    Surfel& surfel, F32& tNear) {
    return MollerTrumbore(ray, meshes, surfel, tNear);
}

bool RayTracer::MollerTrumbore(const Ray& ray, const std::vector<Mesh*>& meshes,
    Surfel& surfel, F32& tNear) {
    // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    Vector3 edge1, edge2, h, s, q;
    F32 k, t, u, v;
    // test against all triangles in the model
    for (UI32 m = 0; m < meshes.size(); ++m) {
        // const Mesh& mesh = model.meshes[m];

        // apply bounding volume test here
        // if (!intersect(ray, mesh.bounds)) then skip mesh;

        for (UI32 f = 0; f < meshes[m]->indices.size(); f+=9) {
            edge1 = meshes[m]->positions[meshes[m]->indices[f+3]]
                - meshes[m]->positions[meshes[m]->indices[f]]; // V1 - v0
            edge2 = meshes[m]->positions[meshes[m]->indices[f+6]]
                - meshes[m]->positions[meshes[m]->indices[f]]; // V2 - v0

            h = ray._direction.cross(edge2);
            k = edge1.dot(h);

            // if cross prod of ray and edge2 is perpendicular to egde1, then 
            // ray is parallel to triangle
            if (k > -EPSILON && k < EPSILON) 
                continue;

            k = 1.0f / k; // reuse k

            // s = v0->origin
            s = ray._origin - meshes[m]->positions[meshes[m]->indices[f]]; 
            u = k * s.dot(h);
            if (u < 0.0f || u > 1.0f)
                continue;

            q = s.cross(edge1);
            v = k * ray._direction.dot(q);
            if (v < 0.0f || u + v > 1.0f)
                continue;

            k = k * edge2.dot(q);
            if (k > EPSILON) { // valid intersection
                t = (ray.At(k) - ray._origin).length(); // test depth
                if (t < tNear) {
                    tNear = t;
                    surfel.tri = f;     // save index of current face
                    surfel.barycentric = {u, v, 1.0f - u - v}; // barycentric coordinates
                    surfel.mesh = meshes[m]; // ptr to mesh data
                }
            }
        }
    }
    return (surfel.mesh != nullptr); // no mesh == no intersection
}

Vector3 RayTracer::RandomAreaLightPoint(const Mesh* light) {
    // get a random triangle in the light mesh
    UI32 index = Random::UniformI32(0, (static_cast<UI32>(light->indices.size()) / 9) - 1);
    // barycentric coordinates
    Vector3 barycentric = UniformSampleTriangle(Random::UniformUV());
    // random point in the triangle
    return 
        light->positions[index  ] * barycentric.x +
        light->positions[index+3] * barycentric.y +
        light->positions[index+3] * barycentric.z;
}

Vector2 RayTracer::UniformSampleDisk(const Vector2& uv) {
    // https://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/2D_Sampling_with_Multidimensional_Transformations
    // to range [-1,1]
    Vector2 p = uv * 2.0f - Vector2{1.0f, 1.0f};
    if (p.x + p.y == 0) // degenerate case
        return Vector2{};
    // generate a random point on a unit disk
    F32 r, theta;
    if (std::abs(p.x) > std::abs(p.y)) {
        r = p.x;
        // distorts grid to avoid collecting at origin
        theta = PI_4 * (p.y / p.x);
    }
    else {
        r = p.y;
        theta = PI_2 - PI_4 * (p.x / p.y);
    }
    return r * Vector2{std::cos(theta), std::sin(theta)};
}

Vector3 RayTracer::UniformSampleHemisphere(const Vector2& uv) {
    // generate a random vector on the unit hemisphere
    F32 r = std::sqrt(std::max(0.0f, 1.0f - uv.x * uv.x));
    F32 phi = 2.0f * PI * uv.y;
    return {r * std::cos(phi), r * std::sin(phi), uv.x};
}

Vector3 RayTracer::CosineSampleHemisphere(const Vector2& uv) {
    Vector2 d = UniformSampleDisk(uv);
    F32 z = std::sqrt(std::max(0.0f, 1.0f - d.x * d.x - d.y * d.y));
    return {d.x, d.y, z};
}

Vector3 RayTracer::UniformSampleTriangleBasuOwen(const F32& u) {
    // u in range [0,1], 1 << 32 = 2^32 (so must be UI64)
    UI32 uFixedPoint = static_cast<UI32>(u * (1ULL << 32));

    Vector2 A{1.0f, 0.0f}, B{0.0f, 1.0f}, C{0.0f, 0.0f};
    Vector2 An, Bn, Cn; // used in iteration
    UI32 digit; // base 4 digit
    for (UI32 i = 0; i < 16; ++i) {
        // extract base4 digit
        digit = (uFixedPoint >> (2 * (15 - i))) & 3; // (u / (15 - i)) % 4
        switch (digit) {
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
    return {An.x, An.y, 1.0f - An.x - An.y};
}

Vector3 RayTracer::UniformSampleTriangle(const Vector2& uv) {
    F32 su0 = std::sqrt(uv.x);
    F32 b0 = 1.0f - su0;
    F32 b1 = uv.y - su0;
    return {b0, b1, 1.0f - b0 - b1};
}
