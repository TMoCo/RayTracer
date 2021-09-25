//
// Raytracer class declaration
//

#include <raytracer/RayTracer.h>

#include <core/Random.h>

RayTracer::RayTracer() {}

void RayTracer::RayTraceImage(Image& buffer, Model* original, 
    Transform t, Camera* camera, UI32 samples) {
    
    // local copy of model
    Model model;
    Model::DeepCopy(*original, model);

    // transform copied model
    for (Mesh& mesh : model.meshes) {
        for (Vector3& position : mesh.positions)
            position = t.TransformPoint(position);
        for (Vector3& normal : mesh.normals)
            normal = t.RotatePoint(normal);
    }

    // scale for aspect ratio
    F32 rWidth = 1.0f / (F32)buffer.Width(), rHeight = 1.0f / (F32)buffer.Height();

    Vector3 eye{0.0f, 0.0f, 0.0f}; // eye at origin

    // primary ray 
    Ray primaryRay;
    primaryRay.origin = eye;

    // gamma correction
    F32 gamma = 1.0f / 2.2f;

    // loop over each pixel in the buffer
    for(UI32 row = 0; row < buffer.Height(); ++row) {
        for (UI32 col = 0; col < buffer.Width(); ++col) {
            //** Fit into a thread
            colour c{};
            // ray direction
            primaryRay.direction = camera->GenerateRay(
                { (col + 0.5f) * rWidth, (row + 0.5f) * rHeight }).Normalize();
                
            // compute scene
            for (UI32 s = 0; s < samples; ++s)
                c += CastRay(primaryRay, model, 0);

            buffer[row][col] = Colour::GammaCorrection(Colour::ReinhardExtendedTMO(c, 10.0f), gamma);
            //**
        }
    }
}

colour RayTracer::CastRay(const Ray& inRay, Model& model, UI32 depth) {
    colour c{};

    if (depth > MAX_DEPTH) // stop recursion 
    {
        //PRINT("reached max depth");
        return c;
    }

    // init variables
    Surfel surfel;
    F32 tNear = INFINITY;

    // intersection of ray with the model
    if(Intersect(inRay, model.objects, surfel, tNear)) {
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
        for (UI32 l = 0; l < model.lights.size(); ++l) {
            tNear = INFINITY;

            // from light to surfel
            ray.origin =  RandomAreaLightPoint(model.lights[l]); // ray originate at light
            ray.direction = (surfel.position - ray.origin).Normalize(); // to surfel
            
            // get intersection of light to surfel
            Intersect(ray, model.objects, shadow, tNear);

            // > same mesh and same triangle = not in shadow
            if ((shadow.mesh == surfel.mesh) && (shadow.tri == surfel.tri)) {
                // compute lighting
                c += 
                    // light colour
                    model.materials.at(model.lights[l]->material).emissive.ToVector3()
                    // surfel brdf
                    * surfel.BRDF(-ray.direction, -inRay.direction, 
                    model.materials.at(surfel.mesh->material))
                    // light distance attenuation
                    / (tNear * tNear);
            }
        }

        // indirect lighting
        ray.direction = Quaternion::RotateVector(
            // random point on hemisphere unit hemisphere
            UniformSampleHemisphere(Random::UniformUV()), 
            // rotation to align with surfel normal
            Quaternion::Rotation(UP, surfel.normal)); 
        
        c += CastRay(ray, model, depth + 1) 
            * surfel.BRDF(ray.direction, -inRay.direction, model.materials.at(surfel.mesh->material));
        /*
        */
    }
    else if(Intersect(inRay, model.lights, surfel, tNear)) 
        c = &model.materials.at(surfel.mesh->material).emissive[0];

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

        for (UI32 f = 0; f < meshes[m]->faces.size(); f+=9) {
            edge1 = meshes[m]->positions[meshes[m]->faces[f+3]] 
                - meshes[m]->positions[meshes[m]->faces[f]]; // V1 - v0
            edge2 = meshes[m]->positions[meshes[m]->faces[f+6]] 
                - meshes[m]->positions[meshes[m]->faces[f]]; // V2 - v0

            h = ray.direction.Cross(edge2);
            k = edge1.Dot(h);

            // if cross prod of ray and edge2 is perpendicular to egde1, then 
            // ray is parallel to triangle
            if (k > -EPSILON && k < EPSILON) 
                continue;

            k = 1.0f / k; // reuse k

            // s = v0->origin
            s = ray.origin - meshes[m]->positions[meshes[m]->faces[f]]; 
            u = k * s.Dot(h);
            if (u < 0.0f || u > 1.0f)
                continue;

            q = s.Cross(edge1);
            v = k * ray.direction.Dot(q);
            if (v < 0.0f || u + v > 1.0f)
                continue;

            k = k * edge2.Dot(q);
            if (k > EPSILON) { // valid intersection
                t = (ray.At(k) - ray.origin).Length(); // test depth
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
    UI32 index = Random::UniformI32(0, (light->faces.size() / 9) - 1);
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
        theta = M_PI_4 * (p.y / p.x);
    }
    else {
        r = p.y;
        theta = M_PI_2 - M_PI_4 * (p.x / p.y);
    }
    return r * Vector2{std::cos(theta), std::sin(theta)};
}

Vector3 RayTracer::UniformSampleHemisphere(const Vector2& uv) {
    // generate a random vector on the unit hemisphere
    F32 r = std::sqrt(std::max(0.0f, 1.0f - uv.x * uv.x));
    F32 phi = 2.0f * M_PI * uv.y;
    return {r * std::cos(phi), r * std::sin(phi), uv.x};
}

Vector3 RayTracer::CosineSampleHemisphere(const Vector2& uv) {
    Vector2 d = UniformSampleDisk(uv);
    F32 z = std::sqrt(std::max(0.0f, 1.0f - d.x * d.x - d.y * d.y));
    return {d.x, d.y, z};
}

Vector3 RayTracer::UniformSampleTriangleBasuOwen(const F32& u) {
    // u in range [0,1], 1 << 32 = 2^32 (so must be UI64)
    UI32 uFixedPoint = u * (1ULL << 32);

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
