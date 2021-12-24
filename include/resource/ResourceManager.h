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

static const std::string MODELS    = "..\\models\\";
static const std::string TEXTURES  =  "..\\materials\\textures\\";
static const std::string MATERIALS =  "..\\materials\\";

class Mesh;
class Material;
class Texture;

class ResourceManager
{
  friend class MaterialLoader;
  friend class OBJLoader;
  friend class SceneLoader;

public:
  static ResourceManager& get();

  Mesh* getMesh(const std::string& meshName) const;
  
  Material* getMaterial(const std::string& materialName) const;

  Texture* getTexture(const std::string& textureName) const;
  
protected:
  void removeMesh(const std::string& meshName);

  void removeMaterial(const std::string& materialName);

  void removeTexture(const std::string& textureName);

  Mesh* addMeshFromFile(const std::string& meshName, const std::string& fileName);

  Material* addMaterialFromFile(const std::string& materialName, const std::string& fileName);

  Texture* addTextureFromFile(const std::string& textureName, const std::string& fileName);

  Mesh* addMesh(const std::string& meshName, Mesh* mesh);

  Material* addMaterial(const std::string& materialName, Material* material);

  Texture* addTexture(const std::string& textureName, Texture* texure);

private:
  ResourceManager();

private:
  std::unordered_map<std::string, Mesh*> meshes;

  std::unordered_map<std::string, Material*> materials;

  std::unordered_map<std::string, Texture*> textures;

};
#endif // !RESOURCE_MANAGER_H
 