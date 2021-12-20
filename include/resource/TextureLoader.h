/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// Texture loader
//

#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <image/Colour.h>

#include <render/Texture.h>

#include <resource/buffer.h>

#include <string>

struct TextureLoader
{
  static bool loadTexture(const std::string& path, Texture& texture, GLenum internalFormat);

  static void writeTexture(const std::string& path, buffer<Colour>& buffer);
  // load different texture files differently
};

#endif // !TEXTURE_LOADER_H
