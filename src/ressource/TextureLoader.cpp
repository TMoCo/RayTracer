#include <resource/TextureLoader.h>

#include <core/debug.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool TextureLoader::loadTexture(const std::string& path, Texture& texture, GLenum internalFormat)
{
  I32 width, height, channels;
  byte* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

  if (data)
  {
    // get texture format from number of channels
    GLenum format = GL_NONE;
    if (channels == 1)
      format = GL_RED;
    else if (channels == 3)
      format = GL_RGB;
    else if (channels == 4)
      format = GL_RGBA;

    DEBUG_ASSERT(format != GL_NONE, "channels %i not supported");

    texture.generate(width, height, internalFormat, GL_RGB, true, data);

    texture.width = width;
    texture.height = height;
  }
  else
    return false;

  stbi_image_free(data);
  return true;
}