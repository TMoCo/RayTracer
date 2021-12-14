/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 14/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <render/shapes/Sphere.h>
#include <render/primitives/GeometricPrimitive.h>

#include <resource/file.h>
#include <resource/SceneLoader.h>

#include <fstream>
#include <cstring>

I32 SceneLoader::loadScene(const std::string& fileName, Scene& scene)
{
  if (!file::isOfType(fileName, ".scene"))
  {
    DEBUG_PRINT("File provided is not .scene!");
    return -1;
  }

  std::ifstream objStream(fileName);

  if (!objStream.is_open())
  {
    DEBUG_PRINT("Failed to open file stream for file %s", fileName.c_str());
    return 1;
  }

  scene.clear(); // make sure scene is empty

  // open file and generate scene
  char line[1024];
  char* token;
  char* remainding;
  std::vector<Node*> nodeStack;

  Node* node;
  Node* newNode;

  UI32 lineNum = 0;
  while (!objStream.eof())
  {
    objStream.getline(line, 1024);

    token = strtok_s(line, " ", &remainding);

    if (token == nullptr) continue;
    
    while (*token == ' ') token++; // chomp whitespace

    if (*token == '#') continue;

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
      
      token = strtok_s(NULL, " ", &remainding); // node name
      node->addChildNode(new Node(token ? token : "geometry", node));
      newNode = node->children.back();
      
      continue;
    }

    if (strcmp(token, "shape") == 0)
    {
      token = strtok_s(NULL, " ", &remainding); // token = shape type, remainding = shape create info
      node->primitive =
        new GeometricPrimitive(createShape(node->getWorldTransform(), token, remainding ? remainding : ""));
      
      continue;
    }

    if (strcmp(token, "position") == 0)
    {
      node->translateNode(Vector3{ strtof(remainding, &token), strtof(token, &token), strtof(token, NULL) });
      continue;
    }
    
    if (strcmp(token, "rotation") == 0)
    {
      node->rotateNode(Quaternion::eulerAngles(strtof(remainding, &token), strtof(token, &token), strtof(token, NULL)));
      continue;
    }

    lineNum++;
  }

  objStream.close();
  // TODO: generate BVH when scene data is loaded
  return 0;
}

Shape* SceneLoader::createShape(Transform* toWorld, const char* shape, char* data)
{
  if (std::strcmp(shape, "sphere") == 0)
  {
    F32 radius = strtof(data, &data);
    F32 zMin = strtof(data, &data);
    F32 zMax = strtof(data, &data);
    F32 phiMax = strtof(data, &data);
    return new Sphere(toWorld, false, radius, zMin, zMax, phiMax);
  }

  if (std::strcmp(shape, "cylindre") == 0)
  {

  }

  if (std::strcmp(shape, "disk") == 0)
  {

  }

  return nullptr;
}