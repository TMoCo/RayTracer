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