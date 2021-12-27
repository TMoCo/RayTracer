/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 26/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// UserInterface class definition
//

#include <widgets/UserInterface.h>
#include <resource/ResourceManager.h>
#include <image/Texture.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

UserInterface::UserInterface()
  : cursorEnabled{ true }, viewNormals{ false }
{ }

UserInterface& UserInterface::get()
{
  static UserInterface ui;
  return ui;
}

I32 UserInterface::processKeyInput(Window* window, F32 deltaTime)
{
  GLFWwindow* w = window->getWindowPointer();
  
  if (glfwGetKey(w, GLFW_KEY_R))
  {
    // ... raytrace scene
    return 0;
  }

  if (glfwGetKey(w, GLFW_KEY_P))
  {
    // .. pause camera movement
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    get().cursorEnabled = true;
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse; // turn flag off
    return 1;
  }

  if (glfwGetKey(w, GLFW_KEY_M))
  {
    // .. start camera movement
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    get().cursorEnabled = false;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse; // turn on
    return 1;
  }

  if (!get().cursorEnabled)
  {
    if (glfwGetKey(w, GLFW_KEY_W))
    {
      window->mainCamera->processInput(Camera::FORWARD, deltaTime * 10.0f);
      return 1;
    }
    if (glfwGetKey(w, GLFW_KEY_S))
    {
      window->mainCamera->processInput(Camera::BACKWARD, deltaTime * 10.0f);
      return 1;
    }
    if (glfwGetKey(w, GLFW_KEY_Q))
    {
      window->mainCamera->processInput(Camera::LEFTWARD, deltaTime * 10.0f);
      return 1;
    }
    if (glfwGetKey(w, GLFW_KEY_D))
    {
      window->mainCamera->processInput(Camera::RIGHTWARD, deltaTime * 10.0f);
      return 1;
    }
  }

  return 1;
}

void UserInterface::mouseCallBack(GLFWwindow* glfwwindow, F64 x, F64 y)
{
  Window* window = (Window*)glfwGetWindowUserPointer(glfwwindow);
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

    if (!get().cursorEnabled)
    {
      F32 sensitivity = 0.1f;
      window->mainCamera->yaw += xoffset * sensitivity;
      window->mainCamera->pitch += yoffset * sensitivity;

      // update camera with new yaw and pitch
      window->mainCamera->update();
    }
  }
}

void UserInterface::init(GLFWwindow* window)
{
  // TODO: move into widgets
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  ImGui::StyleColorsDark();

  glfwSetCursorPosCallback(window, mouseCallBack);

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
}

void UserInterface::set(Application* application)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // gui design here
  ImGui::SetNextWindowSize({ (F32)application->window.getWidth(), (F32)application->window.getHeight() });
  ImGui::SetNextWindowPos({ 0.0f, 0.0f });
  ImGui::Begin("Main", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("New scene"))
    {
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("New background"))
    {
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  ImVec2 region = ImGui::GetContentRegionAvail();
  ImGui::BeginChild("Renders", { region.x * 0.5f, region.y });
  ImGui::Image((void*)(intptr_t)application->window.framebuffer.buffers[0], { region.x * 0.5f, region.y * 0.5f }, { 0,0 }, { 1,-1 }); // OpenGL scene
  Texture* t = ResourceManager::get().getTexture("ray traced output");
  t->bind();
  ImGui::Image((void*)(intptr_t)t->glId, { region.x * 0.5f, region.y * 0.49f }, { 0,0 }, { 1,-1 }); // ray traced
  ImGui::EndChild();

  ImGui::SameLine();

  ImGui::BeginChild("Options", { region.x * 0.5f, region.y }, true);
  
  ImGui::BeginChild("Scene Preview", { region.x * 0.5f, 70.0f });
  {
    ImGui::Text("Scene preview:");
    ImGui::Checkbox("View BVH", &application->drawBVH);
    ImGui::Checkbox("View normals", &viewNormals);
  }
  ImGui::EndChild();
  
  ImGui::Separator();

  ImGui::BeginChild("Ray Tracer Options", { region.x * 0.5f, 100.0f });
  {
    ImGui::Text("Ray Tracer Options:");
  
    I32* numSamples = &application->raytracer.numSamples;
    ImGui::InputInt("Samples num  ", numSamples, 1, 10);
    *numSamples = *numSamples < 1 ? 1 : *numSamples;

    ImGui::Checkbox("Anti Aliasing", &application->antiAliasingEnabled);
    ImGui::SliderFloat("Amount       ", &application->raytracer.antiAliasingKernelSize, 0.0f, 1.0f);
  }
  ImGui::EndChild();
  
  ImGui::Separator();

  ImGui::BeginChild("Camera Options", { region.x * 0.5f, 70.0f });
  {
    ImGui::Text("Camera Options:");
    static F32 ar = 1.0f;
    ImGui::SliderFloat("Aspect ratio ", &ar, 0.0f, 2.0f);
    static F32 FOV = 45.0f;
    ImGui::SliderFloat("FOV          ", &FOV, 0.0f, 90.0f);
  }
  ImGui::EndChild();

  ImGui::Separator();
  
  ImGui::BeginChild("Output Options", { region.x * 0.5f, 70.0f });
  ImGui::Text("Output Options:");
  static char buffer[100] = {};
  ImGui::InputText("File Name", buffer, 100);
  static Vector2 dim;
  ImGui::SliderFloat2("Image Size  ", &dim[0], 500.0f, 4000.0f);
  ImGui::EndChild();

  ImGui::EndChild();

  // ImGui::ShowDemoWindow(&application->debug);

  ImGui::End();
}

void UserInterface::draw()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::terminate()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
