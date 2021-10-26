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

  inline UI32 getWidth() { return width; }

  inline UI32 getHeight() { return height; }

  inline const Camera* getCamera() { return mainCamera; }

  F32 getAspectRatio();
    
  void resize(UI32 width, UI32 height);
 
  static int processInput(Window* window, F32 deltaTime);

  static void resizeCallBack(GLFWwindow* p_win, I32 w, I32 h);

  static void mouseCallBack(GLFWwindow* p_win, F64 x, F64 y);
 
private:
  bool pause = false; // TODO: move into application class

  // mouse move
  bool firstMouse = true;
  F32 lastX, lastY;

  Camera* mainCamera;

  UI32 width, height;
  ViewPort viewPort;
};

#endif // ! WINDOW_H_
