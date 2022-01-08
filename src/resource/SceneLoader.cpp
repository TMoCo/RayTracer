/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <core/core.h>
#include <render/shapes/Sphere.h>
#include <render/primitives/GeometricPrimitive.h>
#include <render/primitives/Mesh.h>
#include <resource/file.h>
#include <resource/SceneLoader.h>
#include <resource/ResourceManager.h>
#include <resource/OBJLoader.h>
#include <scene/Scene.h>

int SceneLoader::loadScene(const std::string& fileName, Scene& scene, bool loadGLdata)
{
  if (!file::isOfType(fileName, ".scene"))
  {
    ERROR_MSG("File provided is not .scene!\n");
    return -1;
  }

  std::ifstream sceneFileStream(fileName);

  if (!sceneFileStream.is_open())
  {
    ERROR_MSG("Failed to open file stream for file %s!", fileName.c_str());
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
      if (node->primitive)
      {
        node->primitive->material = ResourceManager::get().getMaterial(node->name);
      }
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
        Mesh* m = ResourceManager::get().addMeshFromFile(node->name, remainding);
        if (!m)
        {
          ERROR_MSG("Could not load scene! Invalid mesh at: %s", remainding);
        }

        node->setPrimitive(m);

        if (loadGLdata)
        {
          m->generateBuffers(false);
        }

        if (m->nor.size() == 0)
        {
          m->generateNormals();
        }

        m->generateTriangles();

        scene.shapes.reserve(scene.shapes.size() + m->triangles.size());
        for (Triangle* triangle : m->triangles)
        {
          scene.shapes.push_back((Shape*)triangle);
        }
      }
      else
      {
        GeometricPrimitive* p = (GeometricPrimitive*)node->setPrimitive(
          new GeometricPrimitive { createShape(node->getWorldTransform(), token, remainding ? remainding : "") });
        
        p->shape->primitive = p; // YUCK! Todo: change geometric shape constructor

        scene.shapes.push_back(p->shape);
      }

      scene.primitives.push_back(node->primitive);
      continue;
    }

    if (strcmp(token, "material") == 0)
    {
      token = strtok_s(NULL, " ", &remainding); // material path
      Material* material = ResourceManager::get().addMaterialFromFile(node->name, token);
      continue;
    }

    if (strcmp(token, "position") == 0)
    {
      node->local.translateBy({ strtof(remainding, &token), strtof(token, &token), strtof(token, NULL) });
      continue;
    }
    
    if (strcmp(token, "rotation") == 0)
    {
      float x = strtof(remainding, &token), y = strtof(token, &token), z = strtof(token, NULL);
      node->local.rotateBy(Quaternion::eulerAngles(x, y, z));
      continue;
    }

    if (strcmp(token, "camera") == 0)
    {
      scene.mainCamera.ar = strtof(remainding, &token);
      scene.mainCamera.fov = strtof(token, &token);
      scene.mainCamera.n = strtof(token, &token);
      scene.mainCamera.f = strtof(token, NULL);
      continue;
    }

    if (strcmp(token, "lookat") == 0)
    {
      scene.mainCamera.position = { strtof(remainding, &token), strtof(token, &token), strtof(token, &token) };
      Vector3 target = { strtof(token, &token), strtof(token, &token), strtof(token, NULL) };
      scene.mainCamera.front = target == scene.mainCamera.position ? FRONT : (target - scene.mainCamera.position).normalize();
      scene.mainCamera.right = scene.mainCamera.front.cross(UP);
      scene.mainCamera.up = scene.mainCamera.right.cross(scene.mainCamera.front);
      continue;
    }
  }

  sceneFileStream.close();

  return 0;
}

Shape* SceneLoader::createShape(Transform* toWorld, const char* shape, char* data)
{
  if (std::strcmp(shape, "sphere") == 0)
  {
    float radius = strtof(data, &data);
    return new Sphere(toWorld, radius);
  }
  // .. add shapes here ...
  return nullptr;
}