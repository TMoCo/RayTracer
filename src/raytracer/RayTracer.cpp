//
// Raytracer class declaration
//

#include <raytracer/RayTracer.h>

RayTracer::RayTracer() : numSamples(1) {}

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

    F32 wScale, hScale, width, height;

    width = buffer.Width();
    height = buffer.Height();
    F32 aspectRatio = width / height;

    if (aspectRatio > 1.0f) { 
        // portrait
        wScale = 1.0f / width * aspectRatio;
        hScale = 1.0f / height;
    }
    else { 
        // landscape
        wScale = 1.0f / width;
        hScale = 1.0f / height / aspectRatio;
    }

    Vector3 eye{}; // eye at origin
    Ray primaryRay;
    primaryRay.origin = eye;

    // loop over each pixel in the buffer
    for(UI32 row = 0; row < buffer.Height(); ++row) {
        for (UI32 col = 0; col < buffer.Width(); ++col) {
            // ray direction
            primaryRay.direction = Vector3{(2.0f * col - width)  * wScale, 
                (2.0f * row - height) * hScale, -1.8f}.Normalize();

            buffer[row][col] = CastRay(primaryRay, model, 0);
        }
    }
}

rgba_f RayTracer::CastRay(const Ray& ray, Model& model, UI32 depth) {
    // init variables
    rgba_f colour = {0.0f, 0.0f, 0.0f, 1.0f};
    Surfel surfel;
    F32 tNear = INFINITY;

    if(Intersect(ray, model, surfel, tNear)) {
        Vector3 normal = surfel.Normal();
        //colour = (model.materials.at(surfel.mesh->material).ambient / tNear)._v;
        colour = Vector4{normal * 0.5f + Vector3{0.5f, 0.5f, 0.5f}}._v;
    }

    return colour;
}

bool RayTracer::Intersect(const Ray& ray, Model& model, Surfel& surfel, F32& tNear) {
    return MollerTrumbore(ray, model, surfel, tNear);
}

bool RayTracer::MollerTrumbore(const Ray& ray, Model& model, Surfel& surfel, F32& tNear) {
    // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    Vector3 edge1, edge2, h, s, q;
    F32 k, t, u, v;
    // test against all triangles in the model
    for (UI32 m = 0; m < model.meshes.size(); ++m) {
        // const Mesh& mesh = model.meshes[m];

        // apply bounding volume test here
        // if (!intersect(ray, mesh.bounds)) then continue;

        for (UI32 f = 0; f < model.meshes[m].faces.size(); f+=9) {
            edge1 = model.meshes[m].positions[model.meshes[m].faces[f+3]] - model.meshes[m].positions[model.meshes[m].faces[f]]; // V1 - v0
            edge2 = model.meshes[m].positions[model.meshes[m].faces[f+6]] - model.meshes[m].positions[model.meshes[m].faces[f]]; // V2 - v0

            h = ray.direction.Cross(edge2);
            k = edge1.Dot(h);

            // if cross prod of ray and edge2 is perpendicular to egde1, then 
            // ray is parallel to triangle
            if (k > -EPSILON && k < EPSILON) 
                continue;

            k = 1.0f / k; // reuse k

            s = ray.origin - model.meshes[m].positions[model.meshes[m].faces[f]]; // s = v0->origin
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
                    surfel.tri = f; // save index of current face
                    surfel.uv = {u, v}; // barycentric coordinates
                    surfel.mesh = &model.meshes[m]; // ptr to mesh
                }
            }
        }
    }
    return (surfel.mesh != nullptr); // 
}
