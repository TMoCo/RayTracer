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
#include <render/raytracer/Ray.h>
#include <image/Texture.h>

#include <memory>
#include <array>

class Surfel;

typedef enum : uint32_t
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
  virtual bool scatter(const rt::Ray& inRay, const Surfel& surfel, Colour& attenuation, rt::Ray& outRay) const = 0;
  
  virtual Colour emit(const Vector2& uv) const
  {
    return colour::Black;
  }

  virtual void setMap(Texture* map, MAT_MAPS mapType)
  {
    maps[mapType] = map;
  }

protected:
  std::array<Texture*, MAT_MAPS::SIZE> maps{ nullptr, nullptr, nullptr, nullptr };
};

#endif // ! MATERIAL_H
