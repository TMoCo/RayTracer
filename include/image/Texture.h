/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 20/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Texture object
//

#ifndef TEXTURE_H
#define TEXTURE_H 1


#include <glad/glad.h>
#include <image/Image.h>
#include <image/Colour.h>

class Vector2;

class Texture
{
  friend class TextureLoader;
  friend class UserInterface;
public:
  Texture();

  Texture(Image<byte_t>* image, GLenum format);

  ~Texture();

  void bind(int unit = -1);

  Colour sample(const Vector2& uv) const;

  float sampleChannel(const Vector2& uv, int channel) const;

  void generate(bool mip);

  static constexpr float REC_255 = 0.003921568627450980392157f; // 1 / 255;

  GLenum format;

  uint32_t glId;

  Image<byte_t>* image;
  
};

#endif // !TEXTURE_H
