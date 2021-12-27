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

Window::Window(UI32 width = DEFAULT_WIDTH, UI32 height = DEFAULT_HEIGHT, const char* name = "Window")
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
  camera->aspectRatio = (F32)width / (F32)height;
  mainCamera->vpHeight = 2.0f * tan(radians(mainCamera->FOV * 0.5f));
  mainCamera->vpWidth = mainCamera->vpHeight * mainCamera->aspectRatio;
}

F32 Window::getAspectRatio()
{
  return (F32)width / (F32)height;
}

void Window::updateFramebuffer()
{
  framebuffer.destroy();
  framebuffer.build( width, height );
  glViewport(0, 0, width, height);
}

void Window::resizeCallBack(GLFWwindow* p_win, I32 w, I32 h)
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
      window->mainCamera->aspectRatio = (F32)w / (F32)h;
      window->mainCamera->vpHeight = 2.0f * tan(radians(window->mainCamera->FOV * 0.5f));
      window->mainCamera->vpWidth = window->mainCamera->vpHeight * window->mainCamera->aspectRatio;
    }
  }
}