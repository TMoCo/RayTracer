//
// Surfel struct declaration
//

#ifndef SURFEL_H
#define SURFEL_H

#include <core/core.h>

#include <math/Vector2.h>

#include <model/Model.h>

struct Surfel {
    Mesh* mesh; // mesh index
    UI32 tri;   // index of first vertex in surfel coordinates
    Vector2 uv; // barycentric coordinates

    Surfel() : mesh(nullptr), tri(0), uv{} {}

    inline Vector3 Normal() {
        // return the surfel normal
        return 
              uv.x * mesh->normals[mesh->faces[tri+2]]
            + uv.y * mesh->normals[mesh->faces[tri+5]]
            + (1.0f - uv.x - uv.y) * mesh->normals[mesh->faces[tri+8]];
    }
};

#endif // !SURFEL_H