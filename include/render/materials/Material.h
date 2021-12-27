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
#include <image/Texture.h>

#include <memory>
#include <array>

class Ray;
class Surfel;

typedef enum : UI16
{
  ALBEDO,
  NORMAL,
  METALLIC,
  ROUGHNESS,
  EMISSIVE,
  SIZE
} MAT_MAPS;

class Material
{
  friend class MaterialLoader;

public:
  virtual bool scatter(const Ray& inRay, const Surfel& surfel, Colour& attenuation, Ray& outRay) const = 0;
  
  virtual Colour emit(const Vector2& UV) const
  {
    return colour::Black;
  }

  virtual void setMap(Texture* map, MAT_MAPS mapType)
  {
    maps[mapType] = map;
  }

protected:
// TODO: use shader for openGL scene
//  Shader> shader;

  std::array<Texture*, MAT_MAPS::SIZE> maps{ nullptr, nullptr, nullptr, nullptr };
};

#endif // ! MATERIAL_H
