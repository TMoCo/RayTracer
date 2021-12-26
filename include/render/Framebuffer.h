/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 25/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

// FrameBuffer class declaration

#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H 1

#include <core/types.h>

#include <array>

class Framebuffer
{
  enum OutBuffers : UI32
  {
    COLOUR,
    DEPTH,
    // ... add more buffers here
    SIZE
  };

public:
  Framebuffer();

  Framebuffer(UI32 width, UI32 height);

  void bind();

  void bindBuffers();

//private:
public:
  UI32 FBO;
  std::array<UI32, OutBuffers::SIZE> buffers;
};

#endif // !FRAME_BUFFER_H
