//
// OBJLoader class declaration
//

#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H 1

#include <model/Model.h>

#include <string>

#define MAX_LINE 1024 // reading obj, we can expect lines to be smaller than 1024

class OBJLoader {
public:
    static bool LoadObj(const std::string& path, Model& model);
    static bool LoadObj(const char* path, Model& model);

    static bool LoadMtl(const char* path, Model& model);

    static bool IsFileType(const char* path, const char* extension);
    static bool IsObj(const char* path);
    static bool IsMtl(const char* path);
};

#endif // ! OBJ_LOADER_H