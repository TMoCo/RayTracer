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
  friend class Window;

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

  ~Framebuffer();

  void bind();

  void bindBuffers();

protected:
  void build(UI32 width, UI32 height);

  void destroy();

public:
  UI32 FBO;
  std::array<UI32, OutBuffers::SIZE> buffers;
};

#endif // !FRAME_BUFFER_H
