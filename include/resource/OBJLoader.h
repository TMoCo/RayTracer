//
// OBJLoader class declaration
//

#ifndef OBJ_LOADER_H_
#define OBJ_LOADER_H_ 1

#include <scene/Model.h>

#include <string>

class OBJLoader {
public:
    static bool LoadObj(const std::string& path, Model& model);
    static bool LoadObj(const char* path, Model& model);

    static bool LoadMtl(const char* path, Model& model);

    static bool IsFileType(const char* path, const char* extension);
    static bool IsObj(const char* path);
    static bool IsMtl(const char* path);
};

#endif // ! OBJ_LOADER_H_
