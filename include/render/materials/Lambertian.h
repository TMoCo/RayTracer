/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Lambertian material class (purely diffuse)
//

#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H 1

#include <core/random.h>
#include <render/materials/Material.h>
#include <render/raytracer/UniformSampler.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>

class Lambertian : public Material
{
public:
  Lambertian(const Colour& Colour) : albedo{ Colour } {}

  virtual bool scatter(const Ray& inRay, const Surfel& surfel, Colour& attenuation, Ray& outRay) const override
  {
    outRay = { surfel.position, UniformSampler::hemisphere(surfel.normal), INFINITY };
    attenuation = albedo;
    return true;
  }

  // TODO: sample from a texture
  Colour albedo;
};

#endif // !LAMBERTIAN_H