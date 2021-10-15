//
// Texture object
//

#ifndef TEXTURE_H_
#define TEXTURE_H_ 1

#include <core/types.h>

struct Texture
{
  UI32 id;

  void loadTexture(); // onto the GPU

};

#endif // !TEXTURE_H_