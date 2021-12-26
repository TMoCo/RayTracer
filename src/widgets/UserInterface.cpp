/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 26/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// UserInterface class definition
//

#include <widgets/UserInterface.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

UserInterface& UserInterface::get()
{
  static UserInterface ui;
  return ui;
}

I32 UserInterface::processKeyInput(Window* window, F32 deltaTime)
{
  GLFWwindow* w = window->getWindowPointer();
  if (glfwGetKey(w, GLFW_KEY_P))
  {
    // .. pause
    return 1;
  }
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
  if (glfwGetKey(w, GLFW_KEY_R))
  {
    // ... raytrace scene
    return 0;
  }

  return 1;
}

void UserInterface::init(GLFWwindow* window)
{
  // TODO: move into widgets
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  ImGui::StyleColorsDark();

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
    if (ImGui::BeginMenu("File"))
    {
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  ImVec2 region = ImGui::GetContentRegionAvail();
  ImGui::BeginChild("Renders", { region.x * 0.5f, region.y });
  ImGui::Image((void*)(intptr_t)application->framebuffer.buffers[0], { region.x * 0.5f, region.y * 0.5f }, { 0,0 }, { 1,-1 }); // OpenGL scene
  // ImGui::Image() // 
  ImGui::EndChild();

  ImGui::SameLine();

  ImGui::BeginChild("Options", { region.x * 0.5f, region.y });
  
  ImGui::BeginChild("OpenGL Options", { region.x * 0.5f, region.y * 0.5f });
  ImGui::EndChild();

  ImGui::BeginChild("Ray Tracer Options", { region.x * 0.5f, region.y * 0.5f });
  static I32 samples;
  ImGui::SliderInt("Number of Samples", &samples, 0, 100000);
  ImGui::EndChild();

  ImGui::EndChild();

  ImGui::ShowDemoWindow(&application->debug);

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
