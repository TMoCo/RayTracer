/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// A tree data structure for managing a scene
//

#ifndef SCENE_H
#define SCENE_H 1

#include <scene/Node.h>
#include <render/Camera.h>
#include <render/shapes/Shape.h>
#include <render/bounds/LinearBVH.h>
#include <render/raytracer/Ray.h>

class Surfel;

class Scene 
{
public:
  std::string name;

  Scene();

  ~Scene();
  
  void clear(); // dfs delete nodes

  void draw(Shader* shader) const;

  LinearBVH* buildLinearBVH();

  bool intersectLBVH(const rt::Ray& inRay, Surfel* surfel) const;

  bool intersectPrimitives(const rt::Ray& inRay, Surfel* surfel) const;

  Camera mainCamera;

  LinearBVH* linearBVH;

  Node* root;

  std::vector<Primitive*> primitives; // container for all primitives

  std::vector<Shape*> shapes;
};

#endif // !SCENE_H
