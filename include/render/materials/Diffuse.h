/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Lambertian material class (purely diffuse)
//

#ifndef DIFFUSE_H
#define DIFFUSE_H 1

#include <core/random.h>
#include <render/materials/Material.h>
#include <render/raytracer/UniformSampler.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>

class Diffuse : public Material
{
public:
  Diffuse(const Colour& Colour, Texture* albedoMap = nullptr)
    : albedo{ Colour } 
  { }

  virtual bool scatter(const Ray& inRay, const Surfel& surfel, Colour& attenuation, Ray& outRay) const override
  {
    outRay = { surfel.position, UniformSampler::hemisphere(surfel.normal), INFINITY };
    attenuation = maps[MAT_MAPS::ALBEDO] ? maps[MAT_MAPS::ALBEDO]->sample(surfel.uv) : albedo;
    return true;
  }

protected:
  Colour albedo;
};

#endif // !LAMBERTIAN_H