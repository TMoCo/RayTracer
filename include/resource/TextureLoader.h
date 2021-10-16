//
// Texture loader
//

#ifndef TEXTURE_LOADER_H_
#define TEXTURE_LOADER_H_

#include <render/Texture.h>

#include <string>

struct TextureLoader
{
  static bool loadTexture(const std::string& path, Texture& texture, GLenum internalFormat);

  // load different texture files differently
};

#endif // !TEXTURE_LOADER_H_
