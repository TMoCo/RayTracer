//
// Raytracer class 
//

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <core/core.h>

#include <image/Image.h>

#include <model/Model.h>

#include <raytracer/Ray.h>
#include <raytracer/Surfel.h>

#define MAX_DEPTH 4

class RayTracer {
private:
    UI32 numSamples;

public:
    RayTracer();

    void RayTraceImage(Image<rgba_f>& buffer, Model* original, Transform t);
 
private:
    // cast a ray 
    rgba_f CastRay(const Ray& ray, Model& model, UI32 depth);

    // raytracing methods for triangle intersection 
    bool Intersect(const Ray& ray, Model& model, Surfel& surfel, F32& tNear);
    bool MollerTrumbore(const Ray& ray, Model& model, Surfel& surfel, F32& tNear);

    // compute lighting

    // monte carlo sampling (in separate class?)
};

#endif 