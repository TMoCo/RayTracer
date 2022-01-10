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
#include <render/raytracer/sample.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>

class Metal : public Material
{
public:
  Metal(float fuzz, const Colour& Colour) : fuzz{ fuzz < 1.0f ? fuzz : 1.0f }, albedo{ Colour } {}

  virtual bool scatter(const rt::Ray& inRay, const Surfel& surfel, Colour& attenuation, rt::Ray& outRay) const override
  {
    attenuation = colour::White;
    Vector3 outDir = Vector3::reflect(inRay.direction, surfel.normal);
    outRay = { surfel.position + 0.001f * surfel.normal, 
      outDir + fuzz * sample::uniform_unitSphere(random::uniform_uv()), INFINITY };
    return outRay.direction.dot(surfel.normal) > 0.0f;
  }

  float fuzz;

  Colour albedo;
};

#endif // !METAL_H