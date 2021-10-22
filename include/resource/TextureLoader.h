//
// Texture loader
//

#ifndef TEXTURE_LOADER_H_
#define TEXTURE_LOADER_H_

#include <image/colour.h>

#include <render/Texture.h>

#include <resource/buffer.h>

#include <string>

struct TextureLoader
{
  static bool loadTexture(const std::string& path, Texture& texture, GLenum internalFormat);

  static void writeTexture(const std::string& path, buffer<colour>& buffer);
  // load different texture files differently
};

#endif // !TEXTURE_LOADER_H_
