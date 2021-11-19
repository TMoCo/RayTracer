//
// A class for managing a scene
//

#ifndef GEOMETRY_H_
#define GEOMETRY_H_ 1

#include <scene/Transform.h>
#include <scene/Node.h>

#include <render/raytracer/GeometricPrimitive.h>

#include <vector>

class Geometry : protected Node
{
  friend class SceneLoader;
public:
  Geometry() : Node() {}

private:
  Primitive* primitive; // can be an aggregate or different
};

#endif // !GEOMETRY_H_
