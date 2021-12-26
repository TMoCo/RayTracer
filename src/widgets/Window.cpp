/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <widgets/Window.h>
#include <core/debug.h>

Window::Window()
  : width{ 0 }, height{ 0 }, viewPort{ 0, 0, 0, 0 }, lastX{ 0 }, lastY{ 0 },
  mainCamera{ nullptr }, framebuffer{ nullptr }, pWindow{ nullptr }
{ }

Window::Window(UI32 width = DEFAULT_WIDTH, UI32 height = DEFAULT_HEIGHT, const char* name = "Window")
  : width{ width }, height{ height }, viewPort{ 0, 0, width, height }, lastX{ width * 0.5f }, lastY{ height * 0.5f },
  mainCamera{ nullptr }, framebuffer{ nullptr }, pWindow{ glfwCreateWindow(width, height, name, NULL, NULL) }
{
  if (pWindow)
  {
    glfwSetFramebufferSizeCallback(pWindow, resizeCallBack);
    // cursor + mouse
    // glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(pWindow, mouseCallBack);
  }
  else
  {
    ERROR_MSG("Could not create window");
    glfwTerminate();
  }
}

void Window::setViewPort()
{
  glViewport(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
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

void Window::resize(UI32 w, UI32 h)
{
  width = w;
  height = h;
}

void Window::resizeCallBack(GLFWwindow* p_win, I32 w, I32 h)
{
  // get window 
  Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_win));
  if (window)
  {
    window->resize((UI32)w, (UI32)h);
    window->viewPort = { 0, 0, window->width, window->height };
    window->setViewPort();
    if (window->mainCamera) // also update camera
    {
      window->mainCamera->aspectRatio = (F32)w / (F32)h;
      window->mainCamera->vpHeight = 2.0f * tan(radians(window->mainCamera->FOV * 0.5f));
      window->mainCamera->vpWidth = window->mainCamera->vpHeight * window->mainCamera->aspectRatio;
    }
  }
}

void Window::mouseCallBack(GLFWwindow* p_win, F64 x, F64 y)
{
  Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_win));
  if (window)
  {
    if (window->firstMouse) // initially set to true
    {
      window->lastX = (F32)x;
      window->lastY = (F32)y;
      window->firstMouse = false;
    }

    F32 xoffset = (F32)x - window->lastX;
    F32 yoffset = window->lastY - (F32)y;

    window->lastX = (F32)x;
    window->lastY = (F32)y;

    F32 sensitivity = 0.1f;
    window->mainCamera->yaw += xoffset * sensitivity;
    window->mainCamera->pitch += yoffset * sensitivity;

    // update camera with new yaw and pitch
    window->mainCamera->update();
  }
}