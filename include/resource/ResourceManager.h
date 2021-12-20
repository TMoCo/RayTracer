/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*///
// OBJLoader class declaration
//

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H 1

#include <string>
#include <unordered_map>

class Mesh;
class Material;

// TODO: extend to other resource types (textures, materials, composite resources?)
class ResourceManager 
{
  friend class OBJLoader;

  friend class SceneLoader;

public:
  Mesh* tryGetMesh(const std::string& meshName);

  Mesh* getMesh(const std::string& meshName);
  
  Material* tryGetMaterial(const std::string& materialName);

  Material* getMaterial(const std::string& materialName);
  
  void removeMesh(const std::string& meshName);

  void removeMaterial(const std::string& meshName);

private:
  std::unordered_map<std::string, Mesh*> meshes;

  std::unordered_map<std::string, Material*> materials;
};
#endif // !RESOURCE_MANAGER_H
 