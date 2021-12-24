/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Metal material class (reflects light)
//

#ifndef METAL_H
#define METAL_H 1

#include <core/random.h>
#include <render/materials/Material.h>
#include <render/raytracer/UniformSampler.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>

class Metal : public Material
{
public:
  Metal(F32 fuzz, const Colour& Colour) : fuzz{ fuzz < 1.0f ? fuzz : 1.0f }, albedo{ Colour } {}

  virtual bool scatter(const Ray& inRay, const Surfel& surfel, Colour& attenuation, Ray& outRay) const override
  {
    attenuation = maps[MAT_MAPS::ALBEDO] ? maps[MAT_MAPS::ALBEDO]->sample(surfel.uv) : albedo;
    Vector3 outDir = Vector3::reflect(inRay.direction, surfel.normal);
    outRay = { surfel.position, outDir + fuzz * UniformSampler::unitSphere(random::uniformUV()), INFINITY };
    return outRay.direction.dot(surfel.normal) > 0.0f;
  }

  F32 fuzz;

  Colour albedo;
};

#endif // !METAL_H