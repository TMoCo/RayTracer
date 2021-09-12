//
// Model class declaration
//

#ifndef MODEL_H
#define MODEL_H 1

#include <math/Vectors.h>
#include <model/Primitives.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <map>
#include <vector>
#include <cstring>

#define MAX_NAME_LENGTH 128

// POD structs
struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 texture;

    Vertex();
};

struct Material {
    // pod as defined in .mtl
    Vector4 ambient; // Ka
    Vector4 diffuse; // Kd
    Vector4 specular; // Ks
    Vector4 emissive; // Ke
    float dissolve; // d
    float specularExp; // Ns
    float ior; // Ni

    Material();
};

class Object {
public:
    // vertex data
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;

    // face indices into vertex data
    std::vector<uint32_t> faces;

    // primitives
    std::vector<Tri> triangles;
    std::vector<Quad> quads;
    std::vector<Primitive*> primitives;

    // material name
    char material[MAX_NAME_LENGTH];

    // object name
    char name[MAX_NAME_LENGTH];

    Object();

    void DrawTriangles() const;
    void DrawQuads() const;
};

// compare function
struct StrCompare {
    inline bool operator()(const char* l, const char* r) const {
        return std::strcmp(l, r) < 0;
    }
};

class Model {
public:
    std::vector<Object> objects;
    std::map<const char*, Material, StrCompare> materialMap;
    
    Model();

    void Render() const;
    void UseMaterial(const char* material) const;
};


#endif // ! MODEL_H