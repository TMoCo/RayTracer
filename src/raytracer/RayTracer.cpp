//
// Raytracer class declaration
//

#include <raytracer/RayTracer.h>

void RayTracer::RayTraceImage(Image<rgba_f>& buffer, Model* original, const Transform& t) {
    // create a local copy of the model
    Model model;
    Model::DeepCopy(*original, model);

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
    Ray primaryRay{eye};

    // loop over each pixel in the buffer
    for(UI32 row = 0; row < buffer.Height(); ++row) {
        for (UI32 col = 0; col < buffer.Width(); ++col) {
            // ray direction
            primaryRay.direction = Vector3{(2.0f * col - width)  * wScale, 
                (2.0f * row - height) * hScale, -1.8f}.Normalize();
            
            buffer[row][col] = CastRay(primaryRay, model);
        }
    }
}

rgba_f RayTracer::CastRay(const Ray& ray, Model& model) {
    // background colour
    rgba_f colour{ 0.9f, 0.9f, 0.7f, 1.0f };
    F32 tNear = INFINITY;
    Vector2 uv{};
    UI32 tri = 0;
    Mesh* meshPtr = nullptr;

    if(Intersect(ray, model, &meshPtr, tri, uv, tNear)) {
        colour = (model.materials.at(meshPtr->material).ambient / tNear)._v;
    }
    return colour;
}

bool RayTracer::Intersect(const Ray& ray, Model& model, Mesh** meshPtr, 
    UI32& index, Vector2& uv, float& tNear) {
    return MollerTrumbore(ray, model, meshPtr, index, uv, tNear);    
}

bool RayTracer::MollerTrumbore(const Ray& ray, Model& model, Mesh** meshPtr, 
    UI32& index, Vector2& uv, float& tNear) {
    // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    Vector3 edge1, edge2, h, s, q;
    F32 k, u, v;
    // test against all triangles in the model
    for (UI32 m = 0; m < model.meshes.size(); ++m) {
        const Mesh& mesh = model.meshes[m];

        // apply bounding volume test here
        // if (!intersect(ray, mesh.bounds)) then continue;

        for (UI32 f = 0; f < mesh.faces.size(); f+=9) {
            edge1 = mesh.positions[mesh.faces[f+3]] - mesh.positions[mesh.faces[f]]; // V1 - v0
            edge2 = mesh.positions[mesh.faces[f+6]] - mesh.positions[mesh.faces[f]]; // V2 - v0

            h = ray.direction.Cross(edge2);
            k = edge1.Dot(h);

            // if cross prod of ray and edge2 is perpendicular to egde1, then 
            // ray is parallel to triangle
            if (k > -EPSILON && k < EPSILON) 
                continue;

            k = 1.0f / k; // reuse k

            s = ray.origin - mesh.positions[mesh.faces[f]]; // s = v0->origin
            u = k * s.Dot(h);
            if (u < 0.0f || u > 1.0f)
                continue;

            q = s.Cross(edge1);
            v = k * ray.direction.Dot(q);
            if (v < 0.0f || u + v > 1.0f)
                continue;

            k = k * edge2.Dot(q);
            if (k > EPSILON ) {
                index = f; // save index of current face
                uv = {u, v}; // barycentric coordinates
                k = ray.At(k).Length();
                tNear = k < tNear ? k : tNear;
                *meshPtr = &model.meshes[m]; // ptr to mesh
            }
        }
    }
    return (*meshPtr != nullptr); // 
}
