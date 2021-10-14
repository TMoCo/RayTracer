//
// window class definition (assumes glfw has been initialised as part of an application init)
// intended as an interface for managing gltf windows
//

#ifndef WINDOW_H
#define WINDOW_H 1

#include <core/types.h>

#include <render/Camera.h>

#ifdef VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

typedef struct
{
  I32 x, y;
  UI32 width, height;
} ViewPort;

struct Window
{
  I32 width, height;
  ViewPort viewPort;

  Camera* camera;

  GLFWwindow* ptr; // left public for convenience

  int createWindow(UI32 width, UI32 height, const char* name = "Window");

  void setViewPort();

  F32 getAspectRatio();
    
  void resize(UI32 width, UI32 height);
    
  static void resizeCallBack(GLFWwindow* w, int h, int y);
};

#endif // ! WINDOW_H