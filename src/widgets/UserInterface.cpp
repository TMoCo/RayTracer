/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 26/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// UserInterface class definition
//


#include <core/Profiler.h>
#include <core/parallel.h>
#include <core/debug.h>
#include <widgets/UserInterface.h>
#include <resource/ResourceManager.h>

#include <imgui.h>
#include <implot.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

UserInterface::UserInterface()
  : cursorEnabled{ true }, viewNormals{ false }, shouldGenerate{ false }, tasksCount{ 0 }
{ }

UserInterface& UserInterface::get()
{
  static UserInterface ui;
  return ui;
}

int UserInterface::processKeyInput(Window* window, float deltaTime)
{
  GLFWwindow* w = window->getWindowPointer();
  
  if (glfwGetKey(w, GLFW_KEY_ESCAPE))
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
      window->mainCamera->processInput(kCamMove::FORWARD, deltaTime * 10.0f);
      return 1;
    }
    if (glfwGetKey(w, GLFW_KEY_S))
    {
      window->mainCamera->processInput(kCamMove::BACKWARD, deltaTime * 10.0f);
      return 1;
    }
    if (glfwGetKey(w, GLFW_KEY_Q))
    {
      window->mainCamera->processInput(kCamMove::LEFTWARD, deltaTime * 10.0f);
      return 1;
    }
    if (glfwGetKey(w, GLFW_KEY_D))
    {
      window->mainCamera->processInput(kCamMove::RIGHTWARD, deltaTime * 10.0f);
      return 1;
    }
  }

  return 1;
}

void UserInterface::mouseCallBack(GLFWwindow* glfwwindow, double x, double y)
{
  Window* window = (Window*)glfwGetWindowUserPointer(glfwwindow);
  if (window)
  {
    if (window->firstMouse) // initially set to true
    {
      window->lastX = (float)x;
      window->lastY = (float)y;
      window->firstMouse = false;
    }

    float xoffset = (float)x - window->lastX;
    float yoffset = window->lastY - (float)y;

    window->lastX = (float)x;
    window->lastY = (float)y;

    if (!get().cursorEnabled)
    {
      float sensitivity = 0.1f;
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
  ImPlot::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  ImGui::StyleColorsDark();

  glfwSetCursorPosCallback(window, mouseCallBack);

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
}

void UserInterface::set(Application* application, Scene* scene, Texture* rayTraced, Profiler& profiler)
{
  static rt::RayTracerSettings settings{ "out", { 500, 500 }, 1, 1.0f };
  
  // ray traced image
  static float rt_progress = 1.0f;
  static bool rt_finished = true;
  
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  
  // gui design here
  ImGui::SetNextWindowSize({ (float)application->window.getWidth(), (float)application->window.getHeight() });
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
  // OpenGL scene
  ImGui::Image((void*)(intptr_t)application->window.framebuffer.buffers[0], { region.x * 0.5f, region.y * 0.5f }, { 0,0 }, { 1,-1 }); 

  if (shouldGenerate) // every three samples, update gl texture
  {
    rayTraced->generate(true);
    shouldGenerate = false;
  }
  
  ImGui::Image((void*)(intptr_t)rayTraced->glId, { region.x * 0.5f, region.y * 0.49f }, { 0,0 }, { 1,-1 });
  ImGui::EndChild();

  ImGui::SameLine();

  ImGui::BeginChild("Options", { region.x * 0.5f, region.y }, true);
  
  region = ImGui::GetContentRegionAvail();

  ImGui::BeginGroup();
  ImGui::Text("Scene preview:");
  ImGui::Checkbox("View BVH", &application->drawBVH); 
  ImGui::SameLine();
  ImGui::Checkbox("View normals", &viewNormals);
  ImGui::EndGroup();
  
  ImGui::Separator();

  ImGui::BeginGroup();
  ImGui::Text("Ray Tracer Options:");
  ImGui::InputText(".jpg", settings.imageName, sizeof(settings.imageName));
  ImGui::PushItemWidth(region.x * 0.37f);
  int* dim = settings.imgDim; // must be multiple of 4 (GL_RGB format of raytraced image)
  ImGui::InputInt("Width", dim, 4, 4);
  *dim = clamp(*dim, MIN_IMG_SIZE, MAX_IMG_SIZE);
  ImGui::SameLine();
  ImGui::InputInt("Height", dim + 1, 4, 4);
  *(dim + 1) = clamp(*(dim + 1), MIN_IMG_SIZE, MAX_IMG_SIZE);
  ImGui::PopItemWidth();
  int* numSamples = &settings.nSamples;
  ImGui::InputInt("Samples num", numSamples, 1, 10);
  *numSamples = *numSamples < 1 ? 1 : *numSamples;
  ImGui::SliderFloat("Anti-aliasing", &settings.aaKernel, 0.0f, 1.0f);
  if (ImGui::Button("Ray trace image", { region.x, 0 }))
  {
    scene->mainCamera.vpHeight = 2.0f * tanf(radians(scene->mainCamera.fov * 0.5f));
    scene->mainCamera.vpWidth = scene->mainCamera.vpHeight * scene->mainCamera.ar;
    // launch ray trace
    rayTraced->image->clear();
    rt::rayTrace(scene, settings, rayTraced->image, shouldGenerate, tasksCount);
    rt_progress = 0.0f;
    tasksCount = 0;
    rt_finished = false;
  }

  if (tasksCount == parallel::pool.numThreads() * settings.nSamples)
  {
    rayTraced->generate(true); // final update
    rayTraced->image->writeToImageFile(SCREENSHOTS + settings.imageName + ".jpg");
    tasksCount = 0;
    rt_finished = true;
  }

  rt_progress = rt_finished ? 1.0f : tasksCount / (float)(parallel::pool.numThreads() * settings.nSamples);
  ImGui::ProgressBar(rt_progress, { -1.0f, 0.0f });

  ImGui::EndGroup();

  ImGui::Separator();

  ImGui::BeginGroup();
  ImGui::Text("Camera Options:");
  ImGui::SliderFloat("Aspect ratio", &scene->mainCamera.ar, 0.0f, 2.0f);
  ImGui::SliderFloat("Field of view", &scene->mainCamera.fov, 0.0f, 90.0f);
  ImGui::EndGroup();

  ImGui::Separator();
  
  if (ImGui::Button("Run profiler", { region.x * 0.5f, 0 }))
  {
    rt::rayTrace(scene, settings, rayTraced->image, shouldGenerate, tasksCount, &profiler);
  }
  ImGui::SameLine();
  static bool viewPlot = false;
  ImGui::Checkbox("View plot", &viewPlot);
  ImGui::BeginChild("Profiler");
  profiler.drawGui(viewPlot);
  if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
  {
    ImGui::SetScrollHereY(1.0f);
  }
  ImGui::EndChild();

  ImGui::EndChild();
  
  ImPlot::ShowDemoWindow();
#ifndef NDEBUG
    static bool t = true;
    ImGui::ShowDemoWindow(&t);
#endif // !NDEBUG
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
  ImPlot::DestroyContext();
  ImGui::DestroyContext();
}
