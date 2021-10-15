//
// Material structs
//

#ifndef MATERIAL_H_
#define MATERIAL_H_ 1

#include <math/Vector4.h>

#include <cstring>
#include <map>

struct Material {
    // pod as defined in .mtl
    Vector4 ambient; // Ka
    Vector4 diffuse; // Kd
    Vector4 specular; // Ks
    Vector4 emissive; // Ke
    float dissolve; // d
    float specularExp; // Ns
    float ior; // Ni

    Material() : ambient{}, diffuse{}, specular{}, emissive{}, dissolve(0), 
        specularExp(0), ior(0) {}
};

struct StrCompare {
    inline bool operator()(const char* l, const char* r) const {
        return std::strcmp(l, r) == 0;
    }
};

typedef std::map<std::string, Material> MaterialMap;

#endif // ! MATERIAL_H_
