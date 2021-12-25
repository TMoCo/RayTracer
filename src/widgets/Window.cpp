/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <widgets/Window.h>
#include <core/debug.h>

I32 Window::create(UI32 w, UI32 h, const char* name)
{
  // create window
  pWindow = glfwCreateWindow(w, h, name, NULL, NULL);

  if (pWindow)
  {
    // window size + viewport
    viewPort.width  = width  = w;
    viewPort.height = height = h;
    viewPort.x = viewPort.y = 0;
    
    glfwSetFramebufferSizeCallback(pWindow, resizeCallBack);

    // cursor + mouse
    // glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(pWindow, mouseCallBack);
    lastX = width / 2.0f;
    lastY = height / 2.0f;

    return 0;
  }
  else
  {
    ERROR_MSG("Could not create window");
    
    glfwTerminate();

    return -1;
  }
}

void Window::setViewPort()
{
  glViewport(viewPort.x, viewPort.y, viewPort.width, viewPort.height);
}

void Window::setMainCamera(Camera* camera)
{
  mainCamera = camera;
  camera->aspectRatio = width / static_cast<F32>(height);
  mainCamera->vpHeight = 2.0f * tan(radians(mainCamera->FOV * 0.5f));
  mainCamera->vpWidth = mainCamera->vpHeight * mainCamera->aspectRatio;
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

I32 Window::processInput(Window* window, F32 deltaTime, bool pause)
{
  if (glfwGetKey(window->pWindow, GLFW_KEY_ESCAPE))
    return 0;
  // pause
  if (glfwGetKey(window->pWindow, GLFW_KEY_P))
  {
    if (pause)
      glfwSetWindowUserPointer(window->pWindow, nullptr);
    else
    {
      glfwSetWindowUserPointer(window->pWindow, &window);
      window->firstMouse = true;
    }
    return 1;
  }
  if (glfwGetKey(window->pWindow, GLFW_KEY_W))
  {
    window->mainCamera->processInput(Camera::FORWARD, deltaTime * 10.0f);
    return 1;
  }
  if (glfwGetKey(window->pWindow, GLFW_KEY_S))
  {
    window->mainCamera->processInput(Camera::BACKWARD, deltaTime * 10.0f);
    return 1;
  }
  if (glfwGetKey(window->pWindow, GLFW_KEY_Q))
  {
    window->mainCamera->processInput(Camera::LEFTWARD, deltaTime * 10.0f);
    return 1;
  }
  if (glfwGetKey(window->pWindow, GLFW_KEY_D))
  {
    window->mainCamera->processInput(Camera::RIGHTWARD, deltaTime * 10.0f);
    return 1;
  }
  return 1;
}

void Window::resizeCallBack(GLFWwindow* p_win, I32 w, I32 h)
{
  // get window 
  Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_win));
  if (window)
  {
    window->resize(static_cast<UI32>(w), static_cast<UI32>(h));
    window->viewPort = { 0, 0, window->width, window->height };
    window->setViewPort();
    if (window->mainCamera) // also update camera
    {
      window->mainCamera->aspectRatio = static_cast<F32>(w) / static_cast<F32>(h);
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
      window->lastX = static_cast<F32>(x);
      window->lastY = static_cast<F32>(y);
      window->firstMouse = false;
    }

    F32 xoffset = static_cast<F32>(x) - window->lastX;
    F32 yoffset = window->lastY - static_cast<F32>(y);

    window->lastX = static_cast<F32>(x);
    window->lastY = static_cast<F32>(y);

    F32 sensitivity = 0.1f;
    window->mainCamera->yaw += xoffset * sensitivity;
    window->mainCamera->pitch += yoffset * sensitivity;

    // update camera with new yaw and pitch
    window->mainCamera->update();
  }
}