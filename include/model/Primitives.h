//
// Face primitives structs
//

#ifndef PRIMITIVES_H
#define PRIMITIVES_H 1

#include <math/Vector3.h>

#include <cstdint>

#define QUAD_SIZE 12 // 4 vertices * 3 attributes
#define TRI_SIZE 9 // 3 vertices * 3 attributes

enum class kPrimitives : uint32_t {
    NONE = 0,
    TRIANGLE = 3,
    QUAD = 4
};

// primitive structs store offsets into a vector of indices, used to access 
// position, normal and texture vector data

// pure abstract class
struct alignas(0x40) Primitive { // alignas 64 bytes 
    uint32_t _ver;
    uint32_t _tex;
    uint32_t _nor;
    // uint32_t tan

    Primitive() : _ver(0), _tex(0), _nor(0) {}
    Primitive(uint32_t ver, uint32_t tex, uint32_t nor) : 
        _ver(ver), _tex(tex), _nor(nor) {}

    // uint32_t Normal(uint32_t);

    virtual Vector3 Intersect(const Vector3& v, float t) { v * t; return {}; } // keep compiler happy
};

// quad
struct Quad : public Primitive {
    Quad () : Primitive() {}
    Quad (uint32_t ver, uint32_t tex, uint32_t nor) : Primitive{ver, tex, nor} {}

    Vector3 Intersect(const Vector3& v, float t) { v * t; return {}; }
};

// triangle
struct Tri : public Primitive {
    Tri () : Primitive() {}
    Tri (uint32_t ver, uint32_t tex, uint32_t nor) : Primitive{ver, tex, nor} {}

    Vector3 Intersect(const Vector3& v, float t) { v * t; return {}; }
};

#endif