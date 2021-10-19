//
// window class definition (assumes glfw has been initialised as part of an application init)
// intended as an interface for managing gltf windows
//

#ifndef WINDOW_H_
#define WINDOW_H_ 1

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
  bool firstMouse = true;
  F32 lastX, lastY;
  Camera* mainCamera;

  I32 width, height;
  ViewPort viewPort;

  GLFWwindow* ptr; // left public for convenience
  
  int createWindow(UI32 width, UI32 height, const char* name = "Window");

  void setViewPort();

  F32 getAspectRatio();
    
  void resize(UI32 width, UI32 height);
    
  static void resizeCallBack(GLFWwindow* p_win, int w, int h);

  static void mouseCallBack(GLFWwindow* p_win, double x, double y);
};

#endif // ! WINDOW_H_
