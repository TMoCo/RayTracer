//
// Surfel struct declaration
//

#ifndef SURFEL_H
#define SURFEL_H

#include <core/core.h>

#include <image/Colour.h>

#include <math/Vectors.h>

#include <model/Model.h>

struct Surfel {
    Vector3 barycentric; // barycentric coordinates
    Vector3 normal;
    Vector3 position;
    UI32 tri;   // index of first vertex in surfel coordinates
    const Mesh* mesh;
    bool isLight;

    Surfel() : barycentric{}, normal{}, position{}, tri(0), mesh(nullptr) {}

    inline void Interpolate() {
        normal = 
              barycentric.x * mesh->normals[mesh->faces[tri+5]]
            + barycentric.y * mesh->normals[mesh->faces[tri+8]]
            + barycentric.z * mesh->normals[mesh->faces[tri+2]];

        position = 
              barycentric.x * mesh->positions[mesh->faces[tri+3]]
            + barycentric.y * mesh->positions[mesh->faces[tri+6]]
            + barycentric.z * mesh->positions[mesh->faces[tri]]
            + EPSILON * normal; // displace in normal direction
    }

    inline Vector2 UV() {
        // return the surfel uv coordinates
        return 
              barycentric.x * mesh->UVs[mesh->faces[tri+4]]
            + barycentric.y * mesh->UVs[mesh->faces[tri+7]]
            + barycentric.z * mesh->UVs[mesh->faces[tri+1]];
    }

    inline colour BRDF(const Vector3& light, const Vector3& view, const Material& material) {
        // lambertian
        F32 lambertian = std::max(normal.Dot(light), 0.0f);
        // glossy 
        F32 glossy = std::max((light + view).Normalize().Dot(normal), 0.0f);
        // compute surface color
        return material.diffuse.ToVector3()  * lambertian 
            + material.specular.ToVector3() * std::pow(glossy, material.specularExp);
    }
};

#endif // !SURFEL_H