/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 20/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Lambertian material class (purely diffuse)
//

#ifndef TEXTURED_LAMBERTIAN_H
#define TEXTURED_LAMBERTIAN_H 1

#include <core/random.h>
#include <image/Texture.h>
#include <render/materials/Material.h>
#include <render/raytracer/UniformSampler.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>

class TexturedLambertian : public Material
{
public:
  TexturedLambertian(const Texture* texture) : albedo{ texture } {}

  virtual bool scatter(const Ray& inRay, const Surfel& surfel, Colour& attenuation, Ray& outRay) const override
  {
    outRay = { surfel.position, UniformSampler::hemisphere(surfel.normal), INFINITY };
    attenuation = albedo->sample(surfel.uv);
    return true;
  }

  const Texture* albedo;
};

#endif // !TEXTURED_LAMBERTIAN_H