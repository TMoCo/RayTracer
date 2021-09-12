#ifndef PRIMITIVES_H
#define PRIMITIVES_H 1

#include <math/Vector3.h>

#include <cstdint>

enum class kPrimitives : uint32_t {
    NONE = 0,
    TRIANGLE = 3,
    QUAD = 4
};

// pure abstract class
class Primitive {
public:
    uint32_t offset;

    Primitive() : offset(0) {}
    Primitive(uint32_t o) : offset(o) {}

    uint32_t Normal(uint32_t);

    virtual Vector3 Intersect() {}
};

// quad
class Quad : public Primitive {
public:
    static const uint32_t size;

    Quad () : Primitive() {}
    Quad (uint32_t offset) : Primitive(offset) {}
};

// triangle
class Tri : public Primitive { 
public:
    static const uint32_t size;

    Tri () : Primitive() {}
    Tri (uint32_t offset) : Primitive(offset) {}
};

#endif