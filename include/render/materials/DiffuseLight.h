/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 22/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Diffuse Light class 
//

#ifndef DIFFUSE_LIGHT_H
#define DIFFUSE_LIGHT_H 1

#include <core/random.h>
#include <render/materials/Material.h>
#include <render/raytracer/Ray.h>
#include <render/raytracer/Surfel.h>

class DiffuseLight : public Material
{
public:
  DiffuseLight(const Colour& Colour) : emitted{ Colour } {}

  virtual bool scatter(const rt::Ray& inRay, const Surfel& surfel, Colour& attenuation, rt::Ray& outRay) const override
  {
    return false;
  }

  virtual Colour emit(const Vector2& UV) const
  {
    return emitted; // TODO: sample from texture
  }

  Colour emitted;
};

#endif // !DIFFUSE_LIGHT_H 