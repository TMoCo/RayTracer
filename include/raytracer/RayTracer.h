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

#include <random>
#include <chrono>

#define MAX_DEPTH 4

typedef std::uniform_real_distribution<F32> uniform;
typedef  std::chrono::system_clock sysClock;

// random number generation
class Random {
private:
    UI32 seed;

    uniform uniform_0_1;

    std::mt19937 generator;

public:

    inline Random() : seed(sysClock::now().time_since_epoch().count()) {
        uniform_0_1 = uniform(0.0f, 1.0f);
        generator = std::mt19937(seed);
    }

    inline Vector2 UniformVec2_0_1() {
        return Vector2{ uniform_0_1(generator), uniform_0_1(generator) };
    }

    inline F32 UniformF32_0_1() {
        return uniform_0_1(generator);
    }

};

class RayTracer {
private:
    UI32 numSamples;

    Random random;

public:
    RayTracer();

    void RayTraceImage(Image<rgba_f>& buffer, Model* original, Transform t);
 
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