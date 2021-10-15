//
// string utility for file name handling
//

#ifndef FILE_H_
#define FILE_H_

#include <core/debug.h>

#include <string.h>
#include <string>

namespace file
{
#ifdef _WIN32
  constexpr char separator = '\\';
#elif
  constexpr char separator = '/';
#endif

  inline static bool isOfType(const std::string& fileName, const std::string& extension)
  {
    DEBUG_ASSERT(fileName.length() > extension.length(), "");
    // compare the final n chars of name where n = length of extension
    return strcmp(fileName.c_str() + (fileName.length() - extension.length()), extension.c_str()) == 0;
  }

  inline static std::string getFileExtension(std::string fileName)
  {
    size_t pos = fileName.rfind("."); // find last occurence of .
    return fileName.substr(pos == std::string::npos ? fileName.length() : pos); // empty if not found
  }

  inline static std::string getFileName(std::string fileName)
  {
    size_t start = fileName.rfind(separator); // find last occurence of folder separator
    start = start == std::string::npos ? 0 : start + 1;
    size_t end = fileName.rfind("."); // find last period for extension
    return fileName.substr(start, end == std::string::npos ? end : end - start);
  }

  inline static std::string getPath(std::string path)
  {
    size_t pos = path.rfind(separator); // find last occurence of dir separator
    return path.substr(0, pos == std::string::npos ? 0 : pos + 1);
  }
};

#endif // !FILE_H_
