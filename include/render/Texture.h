/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// Texture object
//

#ifndef TEXTURE_H_
#define TEXTURE_H_ 1

#include <core/types.h>

#include <glad/glad.h>

struct Texture
{
  UI32 id;

  I32 width, height;

  void generate(I32 w, I32 h, GLenum internalFormat, GLenum format, bool mip,  const void* data);

  void bind(I32 unit = -1);
};

#endif // !TEXTURE_H_