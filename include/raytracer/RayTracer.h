//
// Raytracer class 
//

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <core/core.h>

#include <image/Image.h>

#include <model/Model.h>

#include <render/Camera.h>

#include <raytracer/Ray.h>
#include <raytracer/Surfel.h>

#define MAX_DEPTH 4

class RayTracer {
public:
    RayTracer();

    void RayTraceImage(Image<rgba_f>& buffer, Model* original, Transform t, 
        Camera* camera, UI32 samples);
 
private:
    // cast a ray 
    Vector4 CastRay(const Ray& ray, Model& model, UI32 depth);

    // raytracing methods for triangle intersection 
    bool Intersect(const Ray& ray, const std::vector<Mesh*>& meshes, 
        Surfel& surfel, F32& tNear);
    bool MollerTrumbore(const Ray& ray, const std::vector<Mesh*>& meshes, 
        Surfel& surfel, F32& tNear);

    // compute lighting
    Vector3 RandomAreaLightPoint(const Mesh* light);
    
    // monte carlo sampling methods
    Vector2 UniformSampleDisk(const Vector2& uv);
    Vector3 UniformSampleHemisphere(const Vector2& uv);
    Vector3 CosineSampleHemisphere(const Vector2& uv);
    // slower but has better variance
    Vector3 UniformSampleTriangleBasuOwen(const F32& u);
    Vector3 UniformSampleTriangle(const Vector2& uv);

};

#endif 