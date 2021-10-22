#include <resource/TextureLoader.h>

#include <core/debug.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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

    DEBUG_ASSERT(format != GL_NONE, "channels %i not supported.");

    texture.generate(width, height, internalFormat, GL_RGB, true, data);

    texture.width = width;
    texture.height = height;
  }
  else
    return false;

  stbi_image_free(data);
  return true;
}

void TextureLoader::writeTexture(const std::string& path, buffer<colour>& b)
{
  // staging buffer 
  buffer<byte> staging{ b.getWidth() * 3, b.getHeight()};

  for (UI32 i = 0; i < b.getWidth(); ++i)
    for (UI32 j = 0; j < b.getHeight(); ++j)
    {
      staging[i*3][j]     = (byte)(b[i][j]).x;
      staging[i*3][j] = (byte)(b[i][j]).y;
      staging[i*3][j] = (byte)(b[i][j]).z;
    }
  
  stbi_write_png(path.c_str(), staging.getWidth(), staging.getHeight(), 3, staging[0], staging.getWidth() * 3);
}