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

class Window
{
public:
  GLFWwindow* ptr; // left public for convenience
  
  int createWindow(UI32 width, UI32 height, const char* name = "Window");

  void setViewPort();

  void setMainCamera(Camera* camera);

  inline const Camera* getCamera() { return mainCamera; }

  F32 getAspectRatio();
    
  void resize(UI32 width, UI32 height);
 
  static int processInput(Window* window, F32 deltaTime);

  static void resizeCallBack(GLFWwindow* p_win, int w, int h);

  static void mouseCallBack(GLFWwindow* p_win, double x, double y);
 
private:
  bool firstMouse = true;
  bool pause = false;
  F32 lastX, lastY;
  Camera* mainCamera;

  I32 width, height;
  ViewPort viewPort;
};

#endif // ! WINDOW_H_
