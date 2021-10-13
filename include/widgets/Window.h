//
// window class definition (assumes glfw has been initialised as part of an application init)
// intended as an interface for managing gltf windows
//

#ifndef WINDOW_H
#define WINDOW_H 1

#include <core/types.h>

#ifdef VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

typedef struct
{
  I32 x, y;
  UI32 width, height;
} ViewPort;

class Window
{
private:
  UI32 w, h; // window dimensions
  ViewPort viewPort;

  void resize(UI32 width, UI32 height);

public:
  GLFWwindow* ptr; // left public for convenience

  int createWindow(UI32 width, UI32 height, const char* name = "Window");

  void setViewPort(ViewPort vp);
    
  static void resizeCallBack(GLFWwindow* w, int h, int y);
};

#endif // ! WINDOW_H