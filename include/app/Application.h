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
#include <render/Framebuffer.h>
#include <scene/Scene.h>

class Application 
{
  friend class UserInterface;

public:
  I32 run(char* sceneName);

private:
  int init();

  void terminate();

  void renderLoop(Scene* scene);

protected:
  Window window;

  RayTracer raytracer;

  bool drawBVH, antiAliasingEnabled;

};

#endif // !APP_H
