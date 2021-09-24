//
// Some core preprocessor directives and constants
//

#ifndef CORE_H
#define CORE_H 1


#include <chrono>
typedef  std::chrono::system_clock sys_clock;

#include <cmath>

// math utils
#define RADIANS(x) \
    x * M_PI / 180.0f

#define NO_COPY(Type) \
    Type(const Type& T) = delete; \
    Type& operator=(const Type& T) = delete;

// application constants
#define SIZEOF_MAT4 0x40
#define SIZEOF_VEC4 0x10
#define SIZEOF_VEC3 0xc
#define SIZEOF_VEC2 0x8
#define EPSILON 0.00001f
#define MAX_NAME_LENGTH 0x80

#include <iostream>

// debug print
#if (__cplusplus >= 202002L)
#define PRINT(format, ...) \
    if (format) \
		std::fprintf(stderr, format __VA__OPT__(,) __VA_ARGS__)
#else
#define PRINT(format, ...) \
    if (format) \
		std::fprintf(stderr, format, ##__VA_ARGS__)
#endif


// assert
#ifndef NDEBUG
	#define m_assert(exp, msg) \
		__m_assert(#exp, exp, __FILE__, __LINE__, msg)
#else
	#define m_assert(exp, msg);
#endif

inline void __m_assert(const char* exp_str, bool exp, const char* file, int line, const char* msg) {
	if (!exp) {
		PRINT("Assertion failed:\t%s\nExpected:\t\t%s\nSource:\t\t\t%s -> line %i\n", msg, exp_str, file, line);
        #ifdef WIN32
            __debugbreak();
        #endif
		abort();
	}
}

#endif // CORE_H