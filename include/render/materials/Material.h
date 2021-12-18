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

class Ray;
class Surfel;

class Material
{
public:
  virtual bool scatter(const Ray& inRay, const Surfel& surfel, Ray& outRay) const = 0;
  
};

#endif // ! MATERIAL_H
