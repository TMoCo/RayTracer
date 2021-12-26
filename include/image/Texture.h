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

#include <core/types.h>
#include <glad/glad.h>
#include <image/Image.h>
#include <image/Colour.h>

class Vector2;

class Texture
{
  friend class TextureLoader;
public:
  Texture();

  Texture(Image* image, GLenum format);

  ~Texture();

  void bind(I32 unit = -1);

  Colour sample(const Vector2& uv) const;

  void generate(bool mip);

public:
  static constexpr F32 REC_255 = 0.003921568627450980392157f; // 1 / 255;

protected:
  GLenum format;

  UI32 glId;

  Image* image;
  
};

#endif // !TEXTURE_H
