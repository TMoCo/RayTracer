/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 27/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Mix material class (can't think of a better name)
//

#ifndef MIXTURE_H
#define MIXTURE_H 1

#include <render/materials/Material.h>

class Mixture : public Material
{
public:
  virtual bool scatter(const Ray& inRay, const Surfel& surfel, Colour& attenuation, Ray& outRay) const override
  {
    F32 metallicity = maps[MAT_MAPS::METALLIC]->sampleChannel(surfel.uv, 0);
    outRay = { surfel.position, UniformSampler::hemisphere(surfel.normal), INFINITY };
    attenuation = maps[MAT_MAPS::NORMAL]->sample(surfel.uv);
    return true;

    if (random::uniformF32() < metallicity)
    {
      // specular
      F32 cosTheta = fmin(surfel.normal.dot(-inRay.direction), 1.0f);
      attenuation = fresnelSchlick(cosTheta, lerp(F0, maps[MAT_MAPS::ALBEDO]->sample(surfel.uv), metallicity));
      
      Vector3 outDir = Vector3::reflect(inRay.direction, surfel.normal);

      outRay = { surfel.position, 
        // roughness affects fuzziness
        outDir + maps[MAT_MAPS::ROUGHNESS]->sampleChannel(surfel.uv, 0) * UniformSampler::unitSphere(random::uniformUV()), 
        INFINITY };

      return true;
    }
    else
    {
      // lambertian
      attenuation = maps[MAT_MAPS::ALBEDO]->sample(surfel.uv);
      outRay = { surfel.position, UniformSampler::hemisphere(surfel.normal), INFINITY };
      return true;
    }
    
  }

  virtual Colour emit(const Vector2& UV) const
  {
    return colour::Black;
  }

  static Colour fresnelSchlick(F32 cosTheta, Colour f0)
  {
    return f0 + (1.0f - f0) * powf(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
  }

private:
  const Colour F0 = { 0.04f };
};

#endif // !MIXTURE_H
