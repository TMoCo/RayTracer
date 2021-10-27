#include <resource/file.h>
#include <resource/SceneLoader.h>

#include <scene/Primitive.h>

#include <fstream>

int SceneLoader::loadScene(const std::string& fileName, Scene* scene)
{
  if (!file::isOfType(fileName, ".scene"))
  {
    DEBUG_PRINT("File provided is not .scene");
    return 1;
  }

  std::ifstream objStream(fileName);

  if (!objStream.is_open())
  {
    DEBUG_PRINT("Could not open file stream for file %s", fileName.c_str());
    return 1;
  }

  if (scene->root != nullptr)
    scene->clear();

  // open file and generate scene
  char line[1024];
  char* token;
  char* remainding;
  std::vector<Node*> nodeStack;

  Node* node;
  Node* newNode;

  while (!objStream.eof())
  {
    objStream.getline(line, 1024);
    token = strtok_s(line, " ", &remainding);
    std::cout << token << "\n";
    std::cout << "'" << remainding << "'\n";

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
      node = nodeStack.back();
      continue;
    }

    if (strcmp(token, "scene") == 0)
    {
      scene->root = new Node;
      token = strtok_s(NULL, " ", &remainding);
      scene->name = token;
      newNode = scene->root;
      continue;
    }
      
    if (strcmp(token, "primitive") == 0)
    {
      DEBUG_ASSERT(scene->root != nullptr, "Can't add a primitive to an empty scene");
      node->addChild(new Primitive);
      newNode = node->children.back();
      token = strtok_s(NULL, " ", &remainding); 
      std::cout << "new primitive: '" << token << "'\n";
      node->name = token;
      continue;
    }

    if (strcmp(token, "shape") == 0)
    {
      // TODO: add check for primitive type
      Primitive* p = reinterpret_cast<Primitive*>(node);
    }

    if (strcmp(token, "position") == 0)
    {
      Vector3 position;
      strtok_s(NULL, " ", &token);
      position.x = strtof(token, &token);
      position.y = strtof(token, &token);
      position.z = strtof(token, &token);
      node->local.position = position;
      continue;
    }
    
    if (strcmp(token, "rotation") == 0)
    {

    }

  }
  objStream.close();
  return 0;
}