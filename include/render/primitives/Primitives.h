//
// Face primitives structs
//

#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_ 1

#include <math/thomath.h>
#include <render/Mesh.h>

struct Primitive 
{
  UI64 index;
  Mesh* mesh;
};

#endif