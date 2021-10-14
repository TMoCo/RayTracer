#include <widgets/Window.h>

#include <core/debug.h>

int Window::createWindow(UI32 w, UI32 h, const char* name)
{
  ptr = glfwCreateWindow(w, h, name, NULL, NULL);
  if (ptr == NULL)
  {
    DEBUG_PRINT("Could not create window");
    glfwTerminate();
    return -1;
  }

  glfwSetFramebufferSizeCallback(ptr, resizeCallBack);
  glfwGetWindowSize(ptr, &width, &height);

  setViewPort();

  return 0;
}

void Window::setViewPort()
{
  glViewport(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
}

F32 Window::getAspectRatio()
{
  return static_cast<F32>(width) / static_cast<F32>(height);
}

void Window::resize(UI32 w, UI32 h)
{
  width = w;
  height = h;
}

void Window::resizeCallBack(GLFWwindow* p_win, int w, int h)
{
  // get window 
  Window* window = static_cast<Window*>(glfwGetWindowUserPointer(p_win));
  if (window)
  {
    window->resize(static_cast<UI32>(w), static_cast<UI32>(h));
  }
}