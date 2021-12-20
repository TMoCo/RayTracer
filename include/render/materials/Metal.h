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
  Metal(const Colour& Colour, F32 fuzz) : albedo{ Colour }, fuzz{ fuzz < 1.0f ? fuzz : 1.0f } {}

  virtual bool scatter(const Ray& inRay, const Surfel& surfel, Colour& attenuation, Ray& outRay) const override
  {
    attenuation = albedo;
    Vector3 outDir = Vector3::reflect(inRay.direction, surfel.normal);
    outRay = { surfel.position, outDir + fuzz * UniformSampler::unitSphere(random::uniformUV()), INFINITY };
    return outRay.direction.dot(surfel.normal) > 0.0f;
  }

  Colour albedo;
  F32 fuzz;
};

#endif // !METAL_H