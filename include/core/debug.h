/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#ifndef DEBUG_H
#define DEBUG_H 1

#ifndef NDEBUG
#include <iostream>
  
// debug print
#if (__cplusplus >= 202002L)
#define DEBUG_PRINT(format, ...) \
  if (format) \
	  fprintf(stderr, format __VA__OPT__(,) __VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) \
  if (format) \
	  fprintf(stderr, format, ##__VA_ARGS__)
#endif

  // assert
#define DEBUG_ASSERT(exp, msg) \
  __assert(#exp, exp, __FILE__, __LINE__, msg)

inline void __assert(const char* exp_str, bool exp, const char* file, int line, const char* msg) 
{
	if (!exp) 
  {
		DEBUG_PRINT("Assertion failed:\t%s\nExpected:\t\t%s\nSource:\t\t\t%s -> line %i\n", msg, exp_str, file, line);
#ifdef WIN32
		__debugbreak();
#endif
		abort();
	}
}

#include <glad/glad.h>
#include <core/core.h>

#define GL_CHECK_ERROR() __glCheckError(__FILE__, __LINE__) 

inline GLenum __glCheckError_(const char* file, int line)
{
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR)
  {
    char error[32];
    switch (errorCode)
    {
    case GL_INVALID_ENUM:
      strcpy_s(error, sizeof(error), "INVALID_ENUM");
      break;
    case GL_INVALID_VALUE:
      strcpy_s(error, sizeof(error), "INVALID_VAL");
      break;
    case GL_INVALID_OPERATION:
      strcpy_s(error, sizeof(error), "INVALID_OP");
      break;
    case GL_STACK_OVERFLOW:
      strcpy_s(error, sizeof(error), "STACK_OVERFLOW");
      break;
    case GL_STACK_UNDERFLOW:
      strcpy_s(error, sizeof(error), "STACK_UNDERFLOW");
      break;
    case GL_OUT_OF_MEMORY:
      strcpy_s(error, sizeof(error), "OUT_OF_MEMORY");
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: 
      strcpy_s(error, sizeof(error), "INVALID_FRAMEBUFFER_OP");
      break;
    }
    __error_msg(file, line, "OPENGL ERROR: %s\n", error);
  }
  return errorCode;
}

#else
// catch certain debug statements and do nothing with them
#define DEBUG_PRINT(...)
#define DEBUG_ASSERT(...)
#endif

#endif // !DEBUG_H
