/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// base Material abstract class
//

#ifndef MATERIAL_H
#define MATERIAL_H 1

#include <image/Colour.h>

class Ray;
class Surfel;

class Material
{
public:
  virtual bool scatter(const Ray& inRay, const Surfel& surfel, Colour& attenuation, Ray& outRay) const = 0;
  
  virtual Colour emit(const Vector2& UV) const
  {
    return colour::Black;
  }
};

#endif // ! MATERIAL_H
