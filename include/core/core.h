//
// Some core preprocessor directives
//

#ifndef CORE_H
#define CORE_H 1

#define NO_COPY(Type) \
    Type(const Type& T) = delete; \
    Type& operator=(const Type& T) = delete;

#define EPSILON 0.00001f
#define MAX_NAME_LENGTH 128

#include <iostream>

#if (__cplusplus >= 202002L)
#define PRINT(format, ...) \
    if (format) \
		std::fprintf(stderr, format __VA__OPT__(,) __VA_ARGS__)
#else
#define PRINT(format, ...) \
    if (format) \
		std::fprintf(stderr, format, ##__VA_ARGS__)
#endif


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