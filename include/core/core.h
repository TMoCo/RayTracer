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

// #define HARD_EXIT

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
  fprintf(stderr, "/!\\ ERROR in %s at line %i!\nInfo:\t", __FILE__, __LINE__);
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
