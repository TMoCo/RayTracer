#include <widgets/Window.h>

#include <core/debug.h>

void Window::resize(UI32 width, UI32 height)
{
  w = width;
  h = height;
}

int Window::createWindow(UI32 width, UI32 height, const char* name)
{
  ptr = glfwCreateWindow(width, height, name, NULL, NULL);
  if (ptr == NULL)
  {
    DEBUG_PRINT("Could not create window");
    glfwTerminate();
    return -1;
  }

  glfwSetFramebufferSizeCallback(ptr, resizeCallBack);

  w = width; 
  h = height;

  setViewPort({ 0, 0, width, height });

  return 0;
}

void Window::setViewPort(ViewPort vp)
{
  viewPort = vp;
  glViewport(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
}

void Window::resizeCallBack(GLFWwindow* window, int width, int height)
{
  // get window 
  Window* user = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if (user)
  {
    user->resize(static_cast<UI32>(width), static_cast<UI32>(height));
  }
}