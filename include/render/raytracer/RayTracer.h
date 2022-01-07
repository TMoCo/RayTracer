/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Raytracer class 
//

#ifndef RAYTRACER_H
#define RAYTRACER_H 1

#include <image/Colour.h>
#include <image/Image.h>
#include <render/Camera.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>
#include <render/primitives/Mesh.h>
#include <scene/Scene.h>
#include <widgets/Window.h>

class RayTracer 
{
  friend class UserInterface;
public:
  RayTracer();

  void rayTrace(const Scene* scene, const Camera* camera);
 
private:
  Colour castRay(const Scene* scene, const Ray& ray, uint32_t depth) const;

protected:
  char outputName[100];

  int32_t dimensions[2];

  Image rayTracedData;

  int32_t numSamples;

  float antiAliasingKernelSize;

  bool antiAliasing;
};

#endif 