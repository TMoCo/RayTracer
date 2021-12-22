/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 20/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Texture loader
//

#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <image/Colour.h>
#include <image/Image.h>
#include <image/Texture.h>

#include <string>

class TextureLoader
{
public:
  static bool loadTextureFromImageFile(const std::string& path, Texture& texture, GLenum internalFormat);
};

#endif // !TEXTURE_LOADER_H
