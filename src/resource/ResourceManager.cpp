/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 18/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <resource/ResourceManager.h>
#include <resource/ObjLoader.h>

Mesh* ResourceManager::tryGetMesh(const std::string& meshName)
{
  // on failure, load mesh
  return meshes.count(meshName) != 0 ? 
    meshes[meshName] : OBJLoader::loadObj(meshName, *this, true) ? 
    meshes[meshName] : nullptr;
}

Mesh* ResourceManager::getMesh(const std::string& meshName)
{
  return meshes.count(meshName) != 0 ? meshes[meshName] : nullptr;
}

void ResourceManager::removeMesh(const std::string& meshName)
{
  meshes.erase(meshName);
}

Material* ResourceManager::tryGetMaterial(const std::string& materialName)
{
  return materials.count(materialName) != 0 ?
    materials[materialName] : nullptr;
}

Material* ResourceManager::getMaterial(const std::string& materialName)
{
  return materials.count(materialName) != 0 ? materials[materialName] : nullptr;
}

void ResourceManager::removeMaterial(const std::string& materialName)
{
  materials.erase(materialName);
}