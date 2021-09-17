//
// Model class declaration
//

#ifndef MODEL_H
#define MODEL_H 1

#include <math/Vectors.h>
#include <model/Material.h>
#include <model/Primitives.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <vector>

#define MAX_NAME_LENGTH 128

class Mesh {
public:
    // vertex data (non interleaved)
    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> UVs;

    // indices
    std::vector<uint32_t> faces;

    // mesh data
    char meshName[MAX_NAME_LENGTH];
    char material[MAX_NAME_LENGTH];

    Mesh();

    void DrawMesh() const;
};

class Model {
public:
    // Meshes
    std::vector<Mesh> meshes;

    // materials
    MaterialMap materials;
    
    Model();

    void Render() const; // loop over meshes and draw
    void UseMaterial(const char* material) const;
};


#endif // ! MODEL_H