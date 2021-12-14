/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/#ifndef APP_H_
#define APP_H_ 1

// define application type here (better way to do this?)

#define OPENGL
// #define VULKAN

#ifdef OPENGL
#include <glad/glad.h>
#endif

#include <resource/ResourceManager.h>

#include <render/raytracer/RayTracer.h>
#include <scene/Scene.h>

#include <widgets/Window.h> 

class Application {
private:
  Window window;

  ResourceManager resourceManager;

  RayTracer raytracer;

  bool pause, debug;

  int init();

  void terminate();

  void renderLoop(Scene* scene);

public:
  I32 run();

};

#endif // !APP_H_
