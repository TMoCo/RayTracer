/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Dielectric material class
//

#ifndef DIELECTRIC_H
#define DIELECTRIC_H 1

#include <render/materials/Material.h>
#include <render/raytracer/UniformSampler.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>

class Dielectric : public Material
{
public:
  Dielectric(F32 ior) : ior{ ior } {}

  virtual bool scatter(const Ray& inRay, const Surfel& surfel, Colour& attenuation, Ray& outRay) const override
  {
    attenuation = colour::White;
    F32 refractionRatio = surfel.isFrontFace ? 1.0f / ior : ior;
    
    F32 cosTheta = fmin(surfel.normal.dot(-inRay.direction), 1.0f);
    F32 sinTheta = sqrtf(1.0f - cosTheta * cosTheta);

    if ((refractionRatio * sinTheta > 1.0f) || (reflectanceSchlick(cosTheta, refractionRatio) > random::uniformF32()))
    {
      outRay = { surfel.position + 0.001f * surfel.normal, Vector3::reflect(inRay.direction, surfel.normal), INFINITY };
    }
    else
    {
      outRay = { surfel.position - 0.001f * surfel.normal, Vector3::refract(inRay.direction, surfel.normal, refractionRatio), INFINITY };
    }

    return true;
  }

private:
  static F32 reflectanceSchlick(F32 cosine, F32 refractionIndex)
  {
    F32 r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
    r0 *= r0;
    return r0 + (1.0f - r0) * powf(1.0f - cosine, 5.0f);
  }

private:
  F32 ior;
};

#endif // !DIELECTRIC_H