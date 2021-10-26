//
// Scene loader class
//

#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H 1

#include <scene/Scene.h>

#include <string>

struct SceneLoader
{
  static int loadScene(const std::string& fileName, Scene* scene);
};

#endif