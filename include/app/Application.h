/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#ifndef APP_H
#define APP_H 1

#include <glad/glad.h>

#include <widgets/Window.h> 
#include <render/raytracer/RayTracer.h>
#include <scene/Scene.h>

class Application {
private:
  Window window;

  RayTracer raytracer;

  bool pause, debug;

  int init();

  void terminate();

  void renderLoop(Scene* scene);

public:
  I32 run();

};

#endif // !APP_H
