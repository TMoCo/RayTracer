/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <math/thomath.h>
#include <image/Texture.h>

Texture::Texture()
  : glId{ 0 }, image{ nullptr }
{ }

Texture::Texture(Image* image)
  : glId{ 0 }, image{ image }
{ }

void Texture::generate(I32 w, I32 h, GLenum internalFormat, GLenum format, bool mip,  const void* data)
{
  glGenTextures(1, &glId);
  bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, data);
  
  if (mip)
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

}

void Texture::bind(I32 unit)
{
  if (unit >= 0)
  {
    glActiveTexture(GL_TEXTURE0 + unit);
  }
  glBindTexture(GL_TEXTURE_2D, glId);
}

Colour Texture::sample(const Vector2& uv) const
{
  const byte* pixel = image->getTexel(uv[0], 1.0f - uv[1]);
  F32 scale = 1.0f / 255.0F;
  return Colour(pixel[0] * scale, pixel[1] * scale, pixel[2] * scale);
}