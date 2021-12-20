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
    Vector3 refracted = Vector3::refract(inRay.direction.normalize(), surfel.normal, refractionRatio);
    outRay = { surfel.position + EPSILON * surfel.normal, refracted, INFINITY };
    return true;
  }

  F32 ior;
};

#endif // !DIELECTRIC_H