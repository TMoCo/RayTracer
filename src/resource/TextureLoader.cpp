/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <resource/TextureLoader.h>
#include <core/debug.h>

#include <stb_image.h>
#include <stb_image_write.h>

bool TextureLoader::loadTextureFromImageFile(const std::string& path, Texture& texture, GLenum internalFormat)
{
  I32 width, height, channels;
  byte* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

  if (data)
  {
    GLenum format = GL_NONE;
    if (channels == 1)
      format = GL_RED;
    else if (channels == 2)
      format = GL_RG;
    else if (channels == 3)
      format = GL_RGB;
    else if (channels == 4)
      format = GL_RGBA;

    if (format == GL_NONE)
    {
      stbi_image_free(data);
      return false;
    }

    texture = Texture{ new Image(width, height, channels, data) };
  }
  else
  {
    return false;
  }

  stbi_image_free(data);
  return true;
}

/*
void TextureLoader::writeImageToFile(const std::string& path, Image& image)
{
  I32 width = static_cast<I32>(image.getWidth()), height = static_cast<I32>(image.getHeight());
  DEBUG_ASSERT(width >= 0 && height >= 0, "");

  // staging Image 
  Image staging{ image.getWidth(), image.getHeight(), image.getChannels() };

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
*/