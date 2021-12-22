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

  Texture(Image* image);

  void bind(I32 unit = -1);

  Colour sample(const Vector2& uv) const;

  void generate(I32 w, I32 h, GLenum internalFormat, GLenum format, bool mip,  const void* data);

// protected:
  UI32 glId;

  Image* image;

};

#endif // !TEXTURE_H
