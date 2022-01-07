/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// Some core preprocessor directives and constants
//

#ifndef CORE_H
#define CORE_H 1

#include <stdio.h>
#include <malloc.h>
#include <chrono>

#ifdef _WIN32
#define WINDOWS 1
#endif // WIN32

#ifdef __APPLE__
#define APPLE 1
#endif

#ifdef __unix__
#define UNIX 1
#endif

#if (__cplusplus >= 202002L)
#define ERROR_MSG(format, ...) \
__m_error_msg(format __VA__OPT__(, ) __VA_ARGS__)
#else
#define ERROR_MSG(format, ...) \
__m_error_msg(__FILE__, __LINE__, format, ##__VA_ARGS__);
#endif

#define CACHE_LINE 64

typedef unsigned char byte_t;
typedef  std::chrono::system_clock sys_clock;

inline void* allocAligned(size_t size)
{
#if WINDOWS
  return _aligned_malloc(size, CACHE_LINE);
#elif defined(APPLE) || defined(UNIX)
  ERROR_MSG("OS not supported!")
  exit(-1);
#endif
}

template <typename T> 
inline T* allocAligned(size_t count)
{
  return (T*)allocAligned(count * sizeof(T));
}

inline void freeAligned(void* block)
{
#if WINDOWS
  _aligned_free(block);
#elif defined(APPLE) || defined(UNIX)
  ERROR_MSG("OS not supported!")
  exit(-1);
#endif
}

inline void __m_error_msg(const char* file, int line, const char* format, ...)
{
  fprintf(stderr, "/!\\ ~~ ERROR ~~ /!\\\nFILE: %s\nLINE: %i\nINFO: ", file, line);
  if (format)
  {
    va_list args;
    __crt_va_start(args, format);
    _vfprintf_p(stderr, format, args); 
    __crt_va_end(args);
  }
  fflush(stderr);
#ifdef WIN32
  __debugbreak();
#endif
}

#endif // CORE_H
