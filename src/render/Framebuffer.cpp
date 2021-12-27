/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 25/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

// FrameBuffer class definition

#include <core/core.h>
#include <render/Framebuffer.h>

#include <glad/glad.h>

Framebuffer::Framebuffer()
  : FBO{ 0 }, buffers{ {} }
{ }

Framebuffer::Framebuffer(UI32 width, UI32 height)
  : FBO{ 0 }, buffers{ {} }
{
  build(width, height);
}

Framebuffer::~Framebuffer()
{
  destroy();
}

void Framebuffer::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Framebuffer::bindBuffers()
{
  glBindTexture(GL_TEXTURE_2D, buffers[OutBuffers::COLOUR]);
}

void Framebuffer::build(UI32 width, UI32 height)
{
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  // colour buffer
  glGenTextures(1, &(buffers[OutBuffers::COLOUR]));
  glBindTexture(GL_TEXTURE_2D, buffers[OutBuffers::COLOUR]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffers[OutBuffers::COLOUR], 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  // depth buffer
  glGenRenderbuffers(1, &(buffers[OutBuffers::DEPTH]));
  glBindRenderbuffer(GL_RENDERBUFFER, buffers[OutBuffers::DEPTH]);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffers[OutBuffers::DEPTH]);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    ERROR_MSG("Framebuffer is not complete!");
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::destroy()
{
  if (buffers[OutBuffers::DEPTH]) glDeleteRenderbuffers(1, &buffers[OutBuffers::DEPTH]);
  if (buffers[OutBuffers::COLOUR]) glDeleteTextures(1, &buffers[OutBuffers::COLOUR]);
  if (FBO) glDeleteFramebuffers(1, &FBO);
}