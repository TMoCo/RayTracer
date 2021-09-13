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

struct Object {
public:
    // primitives
    std::vector<Tri> triangles;
    std::vector<Quad> quads;
    std::vector<Primitive*> primitives;

    // material name
    char material[MAX_NAME_LENGTH];

    // object name
    char name[MAX_NAME_LENGTH];

    Object();
};

class Model {
public:
    // vertex data
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector2> texCoords;

    // face indices into vertex data
    std::vector<uint32_t> faces;

    // objects
    std::vector<Object> objects;
    MaterialMap materials;
    
    Model();

    void Render() const;

    void DrawQuads(const Object& object) const;
    void DrawTris(const Object& object) const;
    void UseMaterial(const char* material) const;
};


#endif // ! MODEL_H