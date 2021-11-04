//
// OBJLoader class declaration
//

#ifndef OBJ_LOADER_H_
#define OBJ_LOADER_H_ 1

#include <render/Mesh.h>

#include <resource/ResourceManager.h>

#include <string>
#include <unordered_map>

// TODO: remove warning sources (strcpy, strtok, size_t to UI32)

class OBJLoader 
{
public:
  static bool loadObj(const std::string& path, ResourceManager& resourceManager, bool singleMesh);

  static bool loadMtl(const std::string& path);

private:
  // used by the obj loader to construct meshes
  struct MeshBuilder
  {
    Mesh* mesh;

    std::unordered_map<std::string, UI32> uniqueIndices;
    
    MeshBuilder() : mesh{ nullptr } {}

    inline void reset(Mesh* m = nullptr)
    {
      uniqueIndices.clear();
      mesh = m; // if provided
    }
  };
};


#endif // ! OBJ_LOADER_H_
 