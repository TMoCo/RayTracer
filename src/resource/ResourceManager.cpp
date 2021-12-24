/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <core/debug.h>
#include <render/primitives/Mesh.h>
#include <render/materials/Material.h>
#include <resource/OBJLoader.h>
#include <resource/MaterialLoader.h>
#include <resource/ResourceManager.h>

ResourceManager::ResourceManager()
{ }

ResourceManager& ResourceManager::get()
{
  static ResourceManager rm;
  return rm;
}

Mesh* ResourceManager::getMesh(const std::string& meshName) const
{
  return meshes.count(meshName) != 0 ? meshes.at(meshName) : nullptr;
}

Material* ResourceManager::getMaterial(const std::string& materialName) const
{
  return materials.count(materialName) != 0 ? materials.at(materialName) : nullptr;
}

Texture* ResourceManager::getTexture(const std::string& textureName) const
{
  return textures.count(textureName) != 0 ? textures.at(textureName) : nullptr;
}

void ResourceManager::removeMesh(const std::string& meshName)
{
  meshes.erase(meshName);
}

void ResourceManager::removeMaterial(const std::string& materialName)
{
  materials.erase(materialName);
}

void ResourceManager::removeTexture(const std::string& textureName)
{
  textures.erase(textureName);
}

Mesh* ResourceManager::addMeshFromFile(const std::string& meshName, const std::string& path)
{
  Mesh* m = getMesh(meshName);
  if (m)
  {
    DEBUG_PRINT("Mesh %s is already loaded!", path.c_str());
    return m;
  }
  OBJLoader::loadOBJFromFile(path, meshName, true);
  return getMesh(meshName);
}

Material* ResourceManager::addMaterialFromFile(const std::string& materialName, const std::string& path)
{
  if (materials.count(materialName) != 0)
  {
    DEBUG_PRINT("Material %s is already loaded!", path.c_str());
    return materials.at(materialName);
  }
  MaterialLoader::loadMaterialFromFile(path, materialName);
  return getMaterial(materialName);
}

Texture* ResourceManager::addTextureFromFile(const std::string& textureName, const std::string& path)
{
  if (textures.count(textureName) != 0)
  {
    DEBUG_PRINT("Texture %s is already loaded!", textureName.c_str());
    return textures.at(textureName);
  }
  MaterialLoader::loadTextureFromImageFile(textureName);
  return getTexture(textureName);
}

Mesh* ResourceManager::addMesh(const std::string& meshName, Mesh* mesh)
{
  if (meshes.count(meshName) != 0)
  {
    removeMesh(meshName);
  }
  meshes[meshName] = mesh;
  return meshes[meshName];
}

Material* ResourceManager::addMaterial(const std::string& materialName, Material* material)
{
  if (materials.count(materialName) != 0)
  {
    removeMaterial(materialName);
  }
  materials[materialName] = material;
  return materials[materialName];
}

Texture* ResourceManager::addTexture(const std::string& textureName, Texture* texture)
{
  if (textures.count(textureName) != 0)
  {
    removeTexture(textureName);
  }
  textures[textureName] = texture;
  return textures[textureName];
}