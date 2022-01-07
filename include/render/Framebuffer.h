/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 25/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

// FrameBuffer class declaration

#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H 1



#include <array>

class Framebuffer
{
  friend class Window;

  enum OutBuffers : uint32_t
  {
    COLOUR,
    DEPTH,
    // ... add more buffers here
    SIZE
  };

public:
  Framebuffer();

  Framebuffer(uint32_t width, uint32_t height);

  ~Framebuffer();

  void bind();

  void bindBuffers();

protected:
  void build(uint32_t width, uint32_t height);

  void destroy();

public:
  uint32_t FBO;
  std::array<uint32_t, OutBuffers::SIZE> buffers;
};

#endif // !FRAME_BUFFER_H
