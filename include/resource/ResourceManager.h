/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// singleton resource manager class
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
  static ResourceManager& get();

  Mesh* getMesh(const std::string& meshName) const;
  
  Material* getMaterial(const std::string& materialName) const;
  
  void removeMesh(const std::string& meshName);

  void removeMaterial(const std::string& materialName);

protected:
  Mesh* addMesh(const std::string& meshName, Mesh* mesh);

  Material* addMaterial(const std::string& materialName, Material* material);

private:
  ResourceManager();

private:
  std::unordered_map<std::string, Mesh*> meshes;

  std::unordered_map<std::string, Material*> materials;

};
#endif // !RESOURCE_MANAGER_H
 