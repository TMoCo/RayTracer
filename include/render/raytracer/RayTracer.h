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

namespace rt
{
  struct RayTracerSettings
  {
    char imageName[128];
    int imgDim[2];
    int nSamples;
    float aaKernel;
    int aaOn;
  };

  void rayTrace(const Scene* scene, RayTracerSettings settings, Image& outImage);

  Colour castRay(const Scene* scene, const Ray& ray, uint32_t depth);
}



class RayTracer 
{
  friend class UserInterface;
public:
  RayTracer();

  void rayTrace(const Scene* scene, bool toGlTexture = true);
 
  char outputName[100];

  int dimensions[2];

  Image rayTracedData;

  int numSamples;

  float antiAliasingKernelSize;

  bool antiAliasing;

private:

};

#endif 