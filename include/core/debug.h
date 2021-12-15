/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/#ifndef DEBUG_H
#define DEBUG_H 1

#ifndef NDEBUG
#include <iostream>
// debug print
#if (__cplusplus >= 202002L)
#define DEBUG_PRINT(format, ...) \
    if (format) \
		std::fprintf(stderr, format __VA__OPT__(,) __VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) \
    if (format) \
		std::fprintf(stderr, format, ##__VA_ARGS__)
#endif


// assert
#define DEBUG_ASSERT(exp, msg) \
		__m_assert(#exp, exp, __FILE__, __LINE__, msg)

inline void __m_assert(const char* exp_str, bool exp, const char* file, int line, const char* msg) {
	if (!exp) {
		DEBUG_PRINT("Assertion failed:\t%s\nExpected:\t\t%s\nSource:\t\t\t%s -> line %i\n", msg, exp_str, file, line);
#ifdef WIN32
		__debugbreak();
#endif
		abort();
	}
}

#else
#include <assert.h>
// catch debug statement and do nothing with them
#define DEBUG_PRINT(...)
#define DEBUG_ASSERT(exp, ...) assert(exp);

#endif

#endif // !DEBUG_H
