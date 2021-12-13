/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// OBJLoader class declaration
//

#ifndef OBJ_LOADER_H_
#define OBJ_LOADER_H_ 1

#include <render/primitives/Mesh.h>

#include <resource/ResourceManager.h>

#include <string>
#include <unordered_map>

class OBJLoader 
{
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

public:
  static bool loadObj(const std::string& path, ResourceManager& resourceManager, bool singleMesh);

  static bool loadMtl(const std::string& path);
};


#endif // ! OBJ_LOADER_H_
 