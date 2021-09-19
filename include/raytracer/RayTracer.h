//
// Raytracer class 
//

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <core/core.h>

#include <image/Image.h>

#include <model/Model.h>

#include <raytracer/Ray.h>

class RayTracer {
public:
    void RayTraceImage(Image<rgba_f>& buffer, Model* original, const Transform& t);
 
private:
    // cast a ray 
    rgba_f CastRay(const Ray& ray, Model& model);

    // raytracing methods for triangle intersection 
    bool Intersect(const Ray& ray, Model& model, Mesh** meshPtr, UI32& index, 
        Vector2& uv, float& tNear);
    bool MollerTrumbore(const Ray& ray, Model& model, Mesh** meshPtr, UI32& index, 
        Vector2& uv, float& tNear);

    // compute lighting

    // monte carlo sampling (in separate class?)
};

#endif 