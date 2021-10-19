//
// Model class declaration
//

#ifndef MODEL_H_
#define MODEL_H_ 1

#include <core/core.h>

#include <math/thomath.h>

#include <render/Material.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
// #include <GL/gl.h>
#endif

#include <vector>
/*

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

    static void DeepCopy(const Mesh& original, Mesh& copy);

    void DrawMesh() const;
};

class Model {
public:
    // Meshes
    std::vector<Mesh> meshes;
    std::vector<Mesh*> lights;  // ptr to meshes that emit light
    std::vector<Mesh*> objects; // ptr to meshes that receive light 

    // materials
    MaterialMap materials;
    
    Model();

    static void DeepCopy(const Model& original, Model& copy);

    void Render() const; // loop over meshes and draw
    void UseMaterial(const char* material) const;
};
*/

#endif // ! MODEL_H_
