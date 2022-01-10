/*
* Author: Thomas Moreno Cooper
* Date created: 10/01/2022
* Copyright 2022 under the MIT license
*/

#ifndef ERROR_H
#define ERROR_H 1

#include <stdio.h>

#if (__cplusplus >= 202002L)
#define ERROR_MSG(format, ...) \
__m_error_msg(format __VA__OPT__(, ) __VA_ARGS__)
#else
#define ERROR_MSG(format, ...) \
__error_msg(__FILE__, __LINE__, format, ##__VA_ARGS__);
#endif

inline void __error_msg(const char* file, int line, const char* format, ...)
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

#endif // !ERROR_H
