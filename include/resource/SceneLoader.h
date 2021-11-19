//
// Scene loader class
//

#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H 1

#include <render/shapes/Shape.h>

#include <scene/Scene.h>

#include <string>

class SceneLoader
{
public:
  static int loadScene(const std::string& fileName, Scene* scene);

private:
  static Shape* createShape(Transform* toWorld, const char* shape, char* data);
};

#endif