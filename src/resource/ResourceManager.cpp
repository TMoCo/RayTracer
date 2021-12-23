/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <resource/ResourceManager.h>

ResourceManager::ResourceManager()
{ }

ResourceManager& ResourceManager::get()
{
  static ResourceManager rm;
  return rm;
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

Mesh* ResourceManager::getMesh(const std::string& meshName) const
{
  return meshes.count(meshName) != 0 ? meshes.at(meshName) : nullptr;
}

void ResourceManager::removeMesh(const std::string& meshName)
{
  meshes.erase(meshName);
}

Material* ResourceManager::getMaterial(const std::string& materialName) const
{
  return materials.count(materialName) != 0 ? materials.at(materialName) : nullptr;
}

void ResourceManager::removeMaterial(const std::string& materialName)
{
  materials.erase(materialName);
}

Material* ResourceManager::addMaterial(const std::string& materialName, Material* material)
{
  return nullptr;
}