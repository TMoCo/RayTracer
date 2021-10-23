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

void Window::setMainCamera(Camera* camera)
{
  mainCamera = camera;
  camera->aspectRatio = width / static_cast<F32>(height);
  mainCamera->vpHeight = 2.0f * tan(RADIANS(mainCamera->FOV * 0.5f));
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

int Window::processInput(Window* window, F32 deltaTime)
{
  if (glfwGetKey(window->ptr, GLFW_KEY_ESCAPE))
    return 0;
  // pause
  if (glfwGetKey(window->ptr, GLFW_KEY_P))
  {
    window->pause = !window->pause;
    if (window->pause)
      glfwSetWindowUserPointer(window->ptr, nullptr);
    else
    {
      glfwSetWindowUserPointer(window->ptr, &window);
      window->firstMouse = true;
    }
    return 1;
  }
  if (glfwGetKey(window->ptr, GLFW_KEY_W))
  {
    window->mainCamera->processInput(Camera::FORWARD, deltaTime * 10.0f);
    return 1;
  }
  if (glfwGetKey(window->ptr, GLFW_KEY_S))
  {
    window->mainCamera->processInput(Camera::BACKWARD, deltaTime * 10.0f);
    return 1;
  }
  if (glfwGetKey(window->ptr, GLFW_KEY_Q))
  {
    window->mainCamera->processInput(Camera::LEFTWARD, deltaTime * 10.0f);
    return 1;
  }
  if (glfwGetKey(window->ptr, GLFW_KEY_D))
  {
    window->mainCamera->processInput(Camera::RIGHTWARD, deltaTime * 10.0f);
    return 1;
  }
  return 1;
}

void Window::resizeCallBack(GLFWwindow* p_win, int w, int h)
{
  // get window 
  Window* window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(p_win));
  if (window)
  {
    window->resize(static_cast<UI32>(w), static_cast<UI32>(h));
    if (window->mainCamera) // also update camera
    {
      window->mainCamera->aspectRatio = static_cast<F32>(w) / static_cast<F32>(h);
      window->mainCamera->vpHeight = 2.0f * tan(RADIANS(window->mainCamera->FOV * 0.5f));
      window->mainCamera->vpWidth = window->mainCamera->vpHeight * window->mainCamera->aspectRatio;
    }
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