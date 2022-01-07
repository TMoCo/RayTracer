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
#include <widgets/UserInterface.h>

Application::Application() 
  : status{ 0 }, drawBVH{ false } 
{
  if (!glfwInit())
  {
    ERROR_MSG("Failed to initialize GLFW!\n");
    status = -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = { DEFAULT_WIDTH, DEFAULT_HEIGHT, "Ray Tracer Window" };
  glfwSetWindowUserPointer(window.getWindowPointer(), &window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    ERROR_MSG("Failed to initialise GLAD!");
    status = -1;
  }

  window.updateFramebuffer();

  UserInterface::get().init(window.getWindowPointer());

  status = 0; // all is well
}

Application::~Application()
{
  UserInterface::get().terminate();

  glfwDestroyWindow(window.getWindowPointer());

  glfwTerminate();
}

int Application::run(char* sceneName)
{
  if (status != 0)
  {
    ERROR_MSG("Failed to initialise app!");
    return -1;
  }

  Scene scene;

  SceneLoader::loadScene(SCENES + sceneName, scene);

  scene.buildBVH();

  renderLoop(&scene);

  return 0;
}

void Application::renderLoop(Scene* scene)
{
  // move into a renderer
  Shader debugShader{ "..\\shaders\\debug.vert", "..\\shaders\\debug.frag" };
  Shader offscreenShader{ "..\\shaders\\vs.vert", "..\\shaders\\fs.frag" };
  Shader compositionShader{ "..\\shaders\\composition.vert", "..\\shaders\\composition.frag" };

  window.setMainCamera(&scene->mainCamera);

  Matrix4 PV; // projection * view
  
  float deltaTime = 0.0f, previous = 0.0f;

  while (!glfwWindowShouldClose(window.getWindowPointer()))
  {
    float current = (float)glfwGetTime();
    deltaTime = current - previous;
    previous = current;

    // INPUT ------------

    if (UserInterface::processKeyInput(&window, deltaTime) == 0)
    {
      scene->mainCamera.vpHeight = 2.0f * tanf(radians(scene->mainCamera.fov * 0.5f));
      scene->mainCamera.vpWidth = scene->mainCamera.vpHeight * scene->mainCamera.ar;
      raytracer.rayTrace(scene, &scene->mainCamera);
    }

    UserInterface::get().set(this, &scene->mainCamera);

    // RENDER SCENE ------------
    window.framebuffer.bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    PV = scene->mainCamera.getProjectionMatrix() * scene->mainCamera.getViewMatrix();
    
    if (drawBVH)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      debugShader.use();
      debugShader.setMatrix4("PV", PV);
      scene->linearBVH->draw(&debugShader);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    offscreenShader.use();
    offscreenShader.setMatrix4("PV", PV);
    scene->draw(&offscreenShader);

    // RENDER GUI ------------
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    UserInterface::get().draw();

    glfwSwapBuffers(window.getWindowPointer());
    glfwPollEvents();
  }
}