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

  // cursor
  glfwSetInputMode(ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(ptr, mouseCallBack);

  glfwSetFramebufferSizeCallback(ptr, resizeCallBack);
  
  glfwGetWindowSize(ptr, &width, &height);

  lastX = width / 2.0f;
  lastY = height / 2.0f;

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
  Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_win));
  if (window)
  {
    window->resize(static_cast<UI32>(w), static_cast<UI32>(h));
  }
}

void Window::mouseCallBack(GLFWwindow* p_win, double x, double y)
{
  Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_win));
  if (window)
  {
    if (window->firstMouse) // initially set to true
    {
      window->lastX = static_cast<F32>(x);
      window->lastY = static_cast<F32>(y);
      window->firstMouse = false;
    }

    F32 xoffset = static_cast<F32>(x) - window->lastX;
    F32 yoffset = window->lastY - static_cast<F32>(y);
    window->lastX = static_cast<F32>(x);
    window->lastY = static_cast<F32>(y);

    F32 sensitivity = 0.1f;
    xoffset *= sensitivity; // yaw
    yoffset *= sensitivity; // pitch

    window->mainCamera->yaw += xoffset;
    window->mainCamera->pitch += yoffset;

    // rotations
    window->mainCamera->update();
  }
}