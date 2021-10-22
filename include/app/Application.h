#ifndef APP_H_
#define APP_H_ 1

// define application type here (better way to do this?)

#define OPENGL
// #define VULKAN

#ifdef OPENGL
#include <glad/glad.h>
#endif

#include <render/raytracer/RayTracer.h>

#include <widgets/Window.h> 

class Application {
private:
  Window window;

  RayTracer raytracer;

  bool pause;
  bool debug;

  int init();

  void terminate();

  // render loops (belong to their respective renderers?)
  void renderLoopGl();

  void renderLoopVk(); // todo: for vulkan applications

  I32 processInput(F32 deltaTime);

public:
  I32 run();

};

#endif // !APP_H_
