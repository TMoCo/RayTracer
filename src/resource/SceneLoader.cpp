/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <render/shapes/Sphere.h>
#include <render/primitives/GeometricPrimitive.h>
#include <render/primitives/Mesh.h>
#include <resource/file.h>
#include <resource/SceneLoader.h>
#include <resource/OBJLoader.h>

#include <cstring>

I32 SceneLoader::loadScene(const std::string& fileName, Scene& scene)
{
  if (!file::isOfType(fileName, ".scene"))
  {
    DEBUG_PRINT("File provided is not .scene!");
    return -1;
  }

  std::ifstream sceneFileStream(fileName);

  if (!sceneFileStream.is_open())
  {
    DEBUG_PRINT("Failed to open file stream for file %s", fileName.c_str());
    return 1;
  }

  scene.clear();

  char line[file::MAX_LINE_SIZE];
  char* token;
  char* remainding;
  std::vector<Node*> nodeStack;

  Node* node;
  Node* newNode;

  while (!sceneFileStream.eof())
  {
    sceneFileStream.getline(line, file::MAX_LINE_SIZE);

    token = strtok_s(line, " ", &remainding);

    if (token == nullptr)
    {
      continue;
    }
    
    while (*token == ' ') 
    { 
      token++; 
    } 

    if (*token == '#') 
    { 
      continue; 
    }

    if (strcmp(token, "{") == 0)
    {
      nodeStack.push_back(newNode);
      node = nodeStack.back();
      continue;
    }

    if (strcmp(token, "}") == 0)
    {
      nodeStack.pop_back();
      node = nodeStack.empty() ? nullptr : nodeStack.back();
      continue;
    }

    if (strcmp(token, "scene") == 0)
    {
      scene.root = new Node("root");
      
      newNode = scene.root;
      
      token = strtok_s(NULL, " ", &remainding);
      scene.name = token ? token : "new scene";
      continue;
    }
      
    if (strcmp(token, "geometry") == 0)
    {
      DEBUG_ASSERT(scene.root != nullptr, "Can't add geometry to an empty scene!");
      token = strtok_s(NULL, " ", &remainding);

      node->addChildNode(new Node(token ? token : "geometry", node));

      newNode = node->children.back();
      continue;
    }

    if (strcmp(token, "shape") == 0)
    {
      token = strtok_s(NULL, " ", &remainding);

      if (strcmp(token, "mesh") == 0)
      {
        OBJLoader::loadObj(remainding, node->name, true);

        Mesh* m = ResourceManager::get().getMesh(node->name);
        m->generateBuffers(false);

        node->setPrimitive(m);
      }
      else
      {
        node->setPrimitive(new GeometricPrimitive
          { createShape(node->getWorldTransform(), token, remainding ? remainding : ""), nullptr });
          // TODO: add material 
      }

      scene.primitives.push_back(node->primitive);
      continue;
    }

    if (strcmp(token, "position") == 0)
    {
      node->local.translateBy({ strtof(remainding, &token), strtof(token, &token), strtof(token, NULL) });
      continue;
    }
    
    if (strcmp(token, "rotation") == 0)
    {
      F32 x = strtof(remainding, &token), y = strtof(token, &token), z = strtof(token, NULL);
      node->local.rotateBy(Quaternion::eulerAngles(x, y, z));
      continue;
    }
  }

  sceneFileStream.close();
  // TODO: generate BVH when scene data is loaded
  return 0;
}

Shape* SceneLoader::createShape(Transform* toWorld, const char* shape, char* data)
{
  if (std::strcmp(shape, "sphere") == 0)
  {
    F32 radius = strtof(data, &data);
    return new Sphere(toWorld, radius);
  }
  
  if (std::strcmp(shape, "cylindre") == 0)
  {

  }

  if (std::strcmp(shape, "disk") == 0)
  {

  }

  return nullptr;
}