/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// OBJLoader class declaration
//

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H 1

#include <render/primitives/Mesh.h>

#include <string>
#include <unordered_map>

// TODO: extend to other resource types (textures, materials, composite resources)

class ResourceManager 
{
  friend class OBJLoader;

  friend class SceneLoader;

public:
  Mesh* tryGetMesh(const std::string& meshName);
  
  Mesh* getMesh(const std::string& meshName);

  void removeMesh(const std::string& meshName);
  
private:
  std::unordered_map<std::string, Mesh*> meshes;
};
#endif // !RESOURCE_MANAGER_H
 