/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// Surfel struct declaration
//

#ifndef SURFEL_H_
#define SURFEL_H_

#include <core/core.h>

#include <render/Mesh.h>

#include <image/Colour.h>

#include <math/thomath.h>

// TODO: Fix surfel to use new mesh

struct Surfel {
    Vector3 barycentric; // barycentric coordinates
    Vector3 normal;
    Vector3 position;
    UI32 tri;   // index of first vertex in surfel coordinates
    const Mesh* mesh;
    bool isLight;

    Surfel() : barycentric{}, normal{}, position{}, tri(0), mesh(nullptr) {}

    inline void Interpolate() {
    }

    inline Vector2 UV() {
    }

    inline colour BRDF(const Vector3& light, const Vector3& view) { // include material
        // lambertian
        F32 lambertian = std::max(normal.dot(light), 0.0f);
        // glossy 
        F32 glossy = std::max((light + view).normalize().dot(normal), 0.0f);
        // compute surface color
        return lambertian * std::pow(glossy, 0.28f);
    }
};

#endif // !SURFEL_H_
