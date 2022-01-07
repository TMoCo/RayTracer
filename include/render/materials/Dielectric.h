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
  Dielectric(float ior) : ior{ ior } {}

  virtual bool scatter(const Ray& inRay, const Surfel& surfel, Colour& attenuation, Ray& outRay) const override
  {
    attenuation = colour::White;
    float refractionRatio = surfel.isFrontFace ? 1.0f / ior : ior;
    
    float cosTheta = fmin(surfel.normal.dot(-inRay.direction), 1.0f);
    float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);

    if ((refractionRatio * sinTheta > 1.0f) || (reflectanceSchlick(cosTheta, refractionRatio) > random::udf_0_1(random::generator)))
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
  static float reflectanceSchlick(float cosine, float refractionIndex)
  {
    float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
    r0 *= r0;
    return r0 + (1.0f - r0) * powf(1.0f - cosine, 5.0f);
  }

private:
  float ior;
};

#endif // !DIELECTRIC_H