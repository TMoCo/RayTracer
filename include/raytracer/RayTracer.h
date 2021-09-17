//
// Raytracer class 
//

#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <image/Image.h>
#include <model/Model.h>
#include <raytracer/Ray.h>

class RayTracer {
public:

    template<typename T>
    void RayTrace(Image<T>& buffer, const Model& model);

private:
    // raytracing methods for primitive intersection test

    // compute lighting

    // monte carlo sampling (in separate class?)
};

#endif 