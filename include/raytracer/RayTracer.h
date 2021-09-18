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
    template<typename T>
    void RayTrace(Image<T>& buffer, Model* original, const Transform& t);

private:
    // raytracing methods for triangle intersection 
    // bool MollerTrumbore(const Ray& ray, UI32)

    // compute lighting

    // monte carlo sampling (in separate class?)
};

template<typename T>
void RayTracer::RayTrace(Image<T>& buffer, Model* original, const Transform& t) {
    // create a local copy of the model
    Model model;
    Model::DeepCopy(*original, model);

    // loop over the positions and normals and transform them
    for (auto& mesh : model.meshes) {
        for (auto& position : mesh.positions) {
            position = t.TransformPoint(position);
        }
        for (auto& normal : mesh.normals) {
            normal = t.RotatePoint(normal);
        }
    }

    // raytrace
}

#endif 