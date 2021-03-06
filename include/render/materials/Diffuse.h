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
#include <render/raytracer/sample.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>

class Diffuse : public Material
{
public:
  Diffuse(const Colour& Colour, Texture* albedoMap = nullptr)
    : albedo{ Colour } 
  { }

  virtual bool scatter(const rt::Ray& inRay, const Surfel& surfel, Colour& attenuation, rt::Ray& outRay) const override
  {
    outRay = { surfel.position + 0.001f * surfel.normal, sample::uniform_hemisphere(surfel.normal), INFINITY };
    attenuation = maps[MAT_MAPS::ALBEDO] ? maps[MAT_MAPS::ALBEDO]->sample(surfel.uv) : albedo;
    return true;
  }

protected:
  Colour albedo;
};

#endif // !LAMBERTIAN_H