#ifndef APP_H
#define APP_H 1

// define application type here (better way to do this?)

#define OPENGL
// #define VULKAN

#ifdef OPENGL
#include <glad/glad.h>
#endif


#include <widgets/Window.h> 

class Application {
private:
  Window window;

  int init();

  void terminate();

  // render loops (belong to their respective renderers?)
  void renderLoopGl();
  void renderLoopVk(); // todo: for vulkan applications

public:
  int run();

};

#endif // !APP_H
