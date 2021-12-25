/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 15/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <app/Application.h>
#include <core/debug.h>
#include <render/bounds/AABB.h>
#include <render/bounds/KDOP.h>
#include <render/materials/materials.h>
#include <render/primitives/BVH.h>
#include <render/primitives/GeometricPrimitive.h>
#include <render/Shader.h>
#include <render/shapes/GLShapes.h>
#include <resource/OBJLoader.h>
#include <resource/MaterialLoader.h>
#include <resource/SceneLoader.h>
#include <resource/ResourceManager.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

int Application::init()
{
  if (!glfwInit())
  {
    ERROR_MSG("Failed to initialize GLFW!\n");
    return -1;
  }

  debug = pause = false;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (window.create(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Ray Tracer Window"))
  {
    ERROR_MSG("Failed to create window!");
    return -1;
  }

  glfwMakeContextCurrent(window.getWindowPointer());

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    ERROR_MSG("Failed to initialise GLAD!");
    return -1;
  }

  // TODO: move into widgets
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window.getWindowPointer(), true);
  ImGui_ImplOpenGL3_Init();


  window.setViewPort();

  return 0;
}

I32 Application::run()
{
  if (init() != 0)
  {
    ERROR_MSG("Failed to initialise app!");
    return -1;
  }

  Scene scene;

  SceneLoader::loadScene("..\\scenes\\triangle.scene", scene);

  renderLoop(&scene);

  terminate();

  return 0;
}

void Application::renderLoop(Scene* scene)
{
  BVH bvh = BVH(scene);

  Mesh* mesh = ResourceManager::get().getMesh("triangle");
  Texture* texture = ResourceManager::get().getTexture("earth.jpg");
  // texture->generate(true);

  // set material
  Shader debugShader{ "..\\shaders\\debug.vert", "..\\shaders\\debug.frag" };
  Shader testShader{ "..\\shaders\\vs.vert", "..\\shaders\\fs.frag" };

  Image rayTracedImage{ window.getWidth(), window.getHeight(), 3 };
  
  Camera camera{ { 0.0f, 0.0f, 0.0f }, 1.0f, 45.0f, 0.1f, 2000.0f };
  window.setMainCamera(&camera);

  Matrix4 PV; // projection * view
  
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  glfwSetWindowUserPointer(window.getWindowPointer(), &window);

  debug = true;

  F32 deltaTime = 0.0f, previous = 0.0f;

  while (!glfwWindowShouldClose(window.getWindowPointer()))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwPollEvents();
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow(&debug);

    F32 current = static_cast<F32>(glfwGetTime());
    deltaTime = current - previous;
    previous = current;

    if (!Window::processInput(&window, deltaTime, pause))
    {
      break;
    }

    PV = camera.getProjectionViewMatrix();

    if (glfwGetKey(window.getWindowPointer(), GLFW_KEY_R))
    {
      // ... raytrace scene
      raytracer.raytrace(scene, rayTracedImage, window.getCamera(), 100);
      rayTracedImage.writeToImageFile("..\\screenshots\\out.jpg");
    }

    // ... draw a mesh
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // texture->bind(0);
    testShader.use();
    testShader.setMatrix4("transform", PV);
    //mesh->draw();
    
    // ... debug
    if (debug)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      debugShader.use();
      debugShader.setMatrix4("VP", PV);
      bvh.draw();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window.getWindowPointer());
  }
}

void Application::terminate()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window.getWindowPointer());

  glfwTerminate();
}