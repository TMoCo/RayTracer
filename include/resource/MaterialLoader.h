/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 20/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Material loader
//

#ifndef MATERIAL_LOADER_H
#define MATERIAL_LOADER_H

#include <image/Colour.h>
#include <image/Image.h>
#include <image/Texture.h>

#include <string>

class MaterialLoader
{
public:
  static bool loadMaterialFromFile(const std::string& path, const std::string& materialName);

  static bool loadTextureFromImageFile(const std::string& path);
};

#endif // !TEXTURE_LOADER_H
