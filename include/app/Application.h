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

class Application 
{
  friend class UserInterface;

public:
  Application();

  ~Application();

  int run(char* sceneName);

private:
  void renderLoop(Scene* scene);

protected:
  Window window;

  RayTracer raytracer;

  int status;

  bool drawBVH;
};

#endif // !APP_H
