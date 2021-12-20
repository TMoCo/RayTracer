/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

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

void TextureLoader::writeTexture(const std::string& path, buffer<Colour>& image)
{
  I32 width = static_cast<I32>(image.getWidth()), height = static_cast<I32>(image.getHeight());
  DEBUG_ASSERT(width >= 0 && height >= 0, "");

  // staging buffer 
  buffer<byte> staging{ image.getWidth(), image.getHeight(), 3 };

  {
    byte* pStaging = staging[0];
    // stb expects image pixels stored from top to bottom, left to right
    for (I32 i = height - 1; i >= 0; --i)
    {
      for (I32 j = 0; j < width; ++j)
      {
        *pStaging       = (byte)(sqrtf(image[i][j][0]) * 255.0f);
        *(pStaging + 1) = (byte)(sqrtf(image[i][j][1]) * 255.0f);
        *(pStaging + 2) = (byte)(sqrtf(image[i][j][2]) * 255.0f);
        pStaging += 3;
      }
    }
  }

  stbi_write_jpg(path.c_str(), width, height, 3, staging[0], 90);
}