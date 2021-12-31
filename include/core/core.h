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

#ifdef _WIN32
#define WINDOWS 1
#endif // WIN32

#ifdef __APPLE__
#define APPLE 1
#endif

#ifdef __unix__
#define UNIX 1
#endif

// #define HARD_EXIT

#define CACHE_LINE 64

inline void* allocAligned(size_t size)
{
#if WINDOWS
  return _aligned_malloc(size, CACHE_LINE);
#elif defined(__APPLE__) || defined(__unix__)
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
#elif defined(__APPLE__) || defined(__unix__)
  ERROR_MSG("OS not supported!")
    exit(-1);
#endif
}

#include <chrono>
typedef  std::chrono::system_clock sys_clock;

#include <iostream>

#if (__cplusplus >= 202002L)
#define ERROR_MSG(format, ...) \
__m_error_msg(format __VA__OPT__(, ) __VA_ARGS__)
#else
#define ERROR_MSG(format, ...) \
__m_error_msg(format, ##__VA_ARGS__);
#endif

inline void __m_error_msg(const char* format, ...)
{
  fprintf(stderr, "/!\\ ERROR!\nInfo:\t");
  if (format)
  {
    // see stdio.h
    va_list args;
    __crt_va_start(args, format);
    _vfprintf_p(stderr, format, args); 
    __crt_va_end(args);
  }
#ifdef WIN32
  __debugbreak();
#endif
#ifdef HARD_EXIT
  abort();
#endif // HARD_EXIT
}

#define NO_COPY(Type) \
  Type(const Type& T) = delete; \
  Type& operator=(const Type& T) = delete;

#define MAX_NAME_LENGTH 0x80 // todo: remove

#endif // CORE_H
