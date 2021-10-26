//
// A class for managing a scene
//

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_ 1

#include <vector>

#include <render/shapes/Shape.h>

#include <scene/Node.h>

struct Primitive : Node
{
  // get the union of all children primitives
  AABB getBounds();

  // primitive data
  Shape* shape;
};

#endif // !PRIMITIVE_H_
