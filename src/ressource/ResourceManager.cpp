#include <resource/ResourceManager.h>
#include <resource/ObjLoader.h>

Mesh* ResourceManager::tryGetMesh(const std::string& meshName)
{
  if (meshes.count(meshName) != 0)
    return meshes[meshName];
  else
  {
    // load mesh
    OBJLoader::loadObj(meshName, *this, true);
  }
}

Mesh* ResourceManager::getMesh(const std::string& meshName)
{
  return meshes.count(meshName) != 0 ? meshes[meshName] : nullptr;
}

void ResourceManager::removeMesh(const std::string& meshName)
{
  meshes.erase(meshName);
}