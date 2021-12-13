/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

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

#define DEFAULT_WIDTH 800U
#define DEFAULT_HEIGHT 600U

typedef struct
{
  I32 x, y;
  UI32 width, height;
} ViewPort;

class Window
{
public:
  I32 create(UI32 width = DEFAULT_WIDTH, UI32 height = DEFAULT_HEIGHT, const char* name = "Window");

  void setViewPort();

  void setMainCamera(Camera* camera);

  inline UI32 getWidth() { return width; }

  inline UI32 getHeight() { return height; }

  inline const Camera* getCamera() { return mainCamera; }

  inline GLFWwindow* getWindowPointer() { return pWindow; }

  F32 getAspectRatio();
    
  void resize(UI32 width, UI32 height);
 
  static I32 processInput(Window* window, F32 deltaTime, bool pause);

  static void resizeCallBack(GLFWwindow* p_win, I32 w, I32 h);

  static void mouseCallBack(GLFWwindow* p_win, F64 x, F64 y);
 
private:
  GLFWwindow* pWindow = nullptr;

  Camera* mainCamera = nullptr;

  UI32 width = 0, height = 0;

  ViewPort viewPort = { 0, 0, 0, 0 };

  F32 lastX = 0, lastY = 0;

  bool firstMouse = true;
};

#endif // ! WINDOW_H_
