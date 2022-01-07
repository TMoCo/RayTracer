/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// window class definition (assumes glfw has been initialised as part of an application init)
// intended as an interface for managing gltf windows
//

#ifndef WINDOW_H
#define WINDOW_H 1

#include <core/types.h>
#include <render/Camera.h>
#include <render/Framebuffer.h>

#include <glad/glad.h>
#ifdef VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#define DEFAULT_WIDTH 800U
#define DEFAULT_HEIGHT 600U

typedef struct
{
  int x, y;
  uint32_t width, height;
} ViewPort;

class Window
{
public:
  Window();

  Window(uint32_t width, uint32_t height, const char* name);

  void setMainCamera(Camera* camera);

  inline uint32_t getWidth() { return width; }

  inline uint32_t getHeight() { return height; }

  inline const Camera* getCamera() { return mainCamera; }

  inline GLFWwindow* getWindowPointer() { return pWindow; }

  float getAspectRatio();

  void updateFramebuffer();
    
  static void resizeCallBack(GLFWwindow* p_win, int w, int h);

public:
  uint32_t width = 0, height = 0;

  ViewPort viewPort = { 0, 0, 0, 0 };

  float lastX = 0, lastY = 0;

  GLFWwindow* pWindow;

  Camera* mainCamera;

  Framebuffer framebuffer;

  bool firstMouse = true;
};

#endif // ! WINDOW_H
