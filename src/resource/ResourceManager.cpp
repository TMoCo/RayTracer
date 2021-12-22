/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <resource/ResourceManager.h>
#include <resource/ObjLoader.h>

Mesh* ResourceManager::getMesh(const std::string& meshName) const
{
  if (meshes.count(meshName) != 0)
  {
    Mesh* mesh = meshes.at(meshName);
    mesh->generateBuffers(false);
    return mesh;
  }
  return nullptr;
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