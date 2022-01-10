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
#include <render/raytracer/Ray.h>
#include <scene/Scene.h>

namespace rt
{
  struct RayTracerSettings
  {
    char imageName[128];
    int imgDim[2];
    int nSamples;
    float aaKernel;
  };

  void rayTrace(const Scene* scene, RayTracerSettings settings, Image* outImage);

  Colour castRay(const Scene* scene, const Ray& ray, uint32_t depth);
}

#endif 