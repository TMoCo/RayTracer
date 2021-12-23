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

#include <chrono>
typedef  std::chrono::system_clock sys_clock;

#include <iostream>

#define ERROR_MSG(msg) \
  std::cerr << "ERROR in " << __FILE__ << " at " << __LINE__ << "\nInfo:"<< msg << std::endl;

#define NO_COPY(Type) \
  Type(const Type& T) = delete; \
  Type& operator=(const Type& T) = delete;

#define MAX_NAME_LENGTH 0x80 // todo: remove

#endif // CORE_H
