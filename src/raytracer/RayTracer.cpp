//
// Raytracer class declaration
//

#include <raytracer/RayTracer.h>

RayTracer::RayTracer() : numSamples(1), random{} {}

void RayTracer::RayTraceImage(Image<rgba_f>& buffer, Model* original, Transform t) {
    // create a local copy of the model
    Model model;
    Model::DeepCopy(*original, model);

    t.Rotate(Quaternion::AngleAxis({0.0f, 1.0f, 0.0f}, RADIANS(-90.0f)));

    // transform model copy data
    for (Mesh& mesh : model.meshes) {
        for (Vector3& position : mesh.positions)
            position = t.TransformPoint(position);
        for (Vector3& normal : mesh.normals)
            normal = t.RotatePoint(normal);
    }

    // scale for aspect ratio
    F32 wScale, hScale, width = buffer.Width(), height = buffer.Height();
    F32 aspectRatio = width / height;
    F32 tanhfov = std::tan(RADIANS(45.0f)); // fixed for testing
    // portrait
    wScale = 1.0f / width * aspectRatio * tanhfov;
    hScale = 1.0f / height * tanhfov;

    Vector3 eye{}; // eye at origin

    // primary ray 
    Ray primaryRay;
    primaryRay.origin = eye;

    F32 gamma = 1.0f / 2.2f;

    // loop over each pixel in the buffer
    for(UI32 row = 0; row < buffer.Height(); ++row) {
        for (UI32 col = 0; col < buffer.Width(); ++col) {
            //** Fit into a thread
                Vector4 colour{};
                // ray direction
                primaryRay.direction = Vector3{
                    (2.0f * col - width) / width * aspectRatio * tanhfov,
                    (2.0f * row - height) / height * tanhfov,
                    -1.0f}.Normalize();
                // std::cout << primaryRay.direction << "\n";
                    // (2.0f * row - height) * hScale, -1.0f}.Normalize();

                F32 tNear = INFINITY;
                Surfel surfel;
                // check if ray intersects a light
                if (Intersect(primaryRay, model.lights, surfel, tNear))
                    colour = model.materials.at(surfel.mesh->material).emissive._v;
                
                // compute scene
                else {
                    for (UI32 samples = 0; samples < numSamples; ++samples)
                        colour += CastRay(primaryRay, model, 0);

                    // tone mapping 
                    colour /= Vector4{1.0f, 1.0f, 1.0f, 1.0f} + colour;
                    // gamma correction
                    colour = { std::pow(colour.x, gamma), std::pow(colour.y, gamma), std::pow(colour.z, gamma) };
                }
                buffer[row][col] = colour._v;
            //**
        }
    }
}

Vector4 RayTracer::CastRay(const Ray& ray, Model& model, UI32 depth) {
    Vector4 colour = {0.0f, 0.0f, 0.0f, 1.0f};

    if (depth > MAX_DEPTH) // stop recursion
        return colour;

    // init variables
    Surfel surfel;
    F32 tNear = INFINITY;

    // intersection of ray with the model
    if(Intersect(ray, model.objects, surfel, tNear)) {
        // compute surfel position, normal and texture coordinates
        surfel.Interpolate();

        // direct lighting
        Surfel shadow;
        Ray lightRay;
        for (UI32 l = 0; l < model.lights.size(); ++l) {
            tNear = INFINITY;

            // from light to surfel
            lightRay.origin =  RandomAreaLightPoint(model.lights[l]); // ray originate at light
            lightRay.direction = (surfel.position - lightRay.origin).Normalize(); 
            
            // get intersection of light to surfel
            Intersect(lightRay, model.objects, shadow, tNear);

            // > same mesh and same triangle = not in shadow
            if (shadow.mesh == surfel.mesh && shadow.tri == surfel.tri) {
                // compute lighting
                colour += 
                    // light colour
                    model.materials.at(model.lights[l]->material).emissive 
                    // surfel brdf
                    * surfel.BRDF(-lightRay.direction, -ray.direction, 
                    model.materials.at(surfel.mesh->material))
                    // light distance attenuation
                    / (tNear * tNear);
            }
        }

        // indirect lighting
        /*
        // from unit to surfel normal
        Quaternion toSurfelNormal = Quaternion::Rotation(UP, normal);

        // indirect lighting
        for (UI32 sample = 0; sample < numSamples; ++sample) {
            ray.direction = Quaternion::RotateVector(
                UniformSampleHemisphere(random.UniformVec2_0_1()), // random point on hemisphere
                toSurfelNormal); // align with surface
            // CastRay(ray, model, depth + 1);
        }
        */
    }

    return colour;
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
    UI32 index = 9 * (random.UniformF32_0_1() * (light->faces.size() / 9));
    // barycentric coordinates
    // Vector3 barycentric = UniformSampleTriangle(random.UniformVec2_0_1());
    Vector3 barycentric = UniformSampleTriangle(random.UniformVec2_0_1());
    // random point in the triangle
    return 
        light->positions[index+3] * barycentric.x +
        light->positions[index+6] * barycentric.y +
        light->positions[index  ] * barycentric.z;
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
