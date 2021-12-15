/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// Some core preprocessor directives and constants
//

#ifndef CORE_H_
#define CORE_H_ 1

#include <chrono>
typedef  std::chrono::system_clock sys_clock;

#define NO_COPY(Type) \
    Type(const Type& T) = delete; \
    Type& operator=(const Type& T) = delete;

#define MAX_NAME_LENGTH 0x80 // todo: remove

#endif // CORE_H_
