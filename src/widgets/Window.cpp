/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <widgets/Window.h>
#include <core/debug.h>

Window::Window()
  : width{ 0 }, height{ 0 }, viewPort{ 0, 0, 0, 0 }, lastX{ 0 }, lastY{ 0 },
  mainCamera{ nullptr }, pWindow{ nullptr }
{ }

Window::Window(uint32_t width = DEFAULT_WIDTH, uint32_t height = DEFAULT_HEIGHT, const char* name = "Window")
  : width{ width }, height{ height }, viewPort{ 0, 0, width, height }, lastX{ width * 0.5f }, lastY{ height * 0.5f },
  mainCamera{ nullptr }, pWindow{ glfwCreateWindow(width, height, name, NULL, NULL) }
{
  if (pWindow)
  {
    glfwSetFramebufferSizeCallback(pWindow, resizeCallBack);
  }
  else
  {
    ERROR_MSG("Could not create window");
    glfwTerminate();
  }
}

void Window::setMainCamera(Camera* camera)
{
  mainCamera = camera;
  camera->aspectRatio = (float)width / (float)height;
  mainCamera->vpHeight = 2.0f * tan(radians(mainCamera->FOV * 0.5f));
  mainCamera->vpWidth = mainCamera->vpHeight * mainCamera->aspectRatio;
}

float Window::getAspectRatio()
{
  return (float)width / (float)height;
}

void Window::updateFramebuffer()
{
  framebuffer.destroy();
  framebuffer.build( width, height );
  glViewport(0, 0, width, height);
}

void Window::resizeCallBack(GLFWwindow* p_win, int w, int h)
{
  // get window 
  Window* window = (Window*)(glfwGetWindowUserPointer(p_win));
  if (window)
  {
    window->width = w;
    window->height = h;
    // window->updateFramebuffer();

    if (window->mainCamera) // also update camera
    {
      window->mainCamera->aspectRatio = (float)w / (float)h;
      window->mainCamera->vpHeight = 2.0f * tan(radians(window->mainCamera->FOV * 0.5f));
      window->mainCamera->vpWidth = window->mainCamera->vpHeight * window->mainCamera->aspectRatio;
    }
  }
}