//
// OBJLoader class declaration
//

#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_ 1

#include <render/Mesh.h>

#include <string>
#include <unordered_map>

// TODO: extend to other resource types (textures, materials, composite resources)

class ResourceManager 
{
  friend class OBJLoader;
public:
  Mesh* tryGetMesh(const std::string& meshName);
  
  Mesh* getMesh(const std::string& meshName);

  void removeMesh(const std::string& meshName);
  
private:
  std::unordered_map<std::string, Mesh*> meshes;
};
#endif // !RESOURCE_MANAGER_H_
 