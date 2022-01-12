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
#include <atomic>

class Profiler;

namespace rt
{
  enum kAcceleration : uint32_t
  {
    NONE = 0x0u,
    PARALLEL = 0x1u,
    LBVH = 0x2u
  };

  struct RayTracerSettings
  {
    char imageName[128];
    int imgDim[2];
    int nSamples;
    float aaKernel;
  };

  void rayTraceFast(const Scene* scene, RayTracerSettings settings, Image<byte_t>* raytraced);

  void rayTraceSlow(const Scene* scene, RayTracerSettings settings, Image<byte_t>* raytraced, std::atomic<bool>& updateTex,
    std::atomic<int>& taskCount, Profiler* profiler = nullptr,uint32_t accelMask = 0x3u);

  Colour castRay(const Scene* scene, const Ray& ray, uint32_t depth);
  
  Colour castRayBVH(const Scene* scene, const Ray& ray, uint32_t depth);

  float castProfilerRay(const Scene* scene, const Ray& inRay, float depth);
}

#endif 