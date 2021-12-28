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

int Application::init()
{
  if (!glfwInit())
  {
    ERROR_MSG("Failed to initialize GLFW!\n");
    return -1;
  }

  drawBVH = false, antiAliasingEnabled = false;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = Window{ DEFAULT_WIDTH, DEFAULT_HEIGHT, "Ray Tracer Window" };

  glfwMakeContextCurrent(window.getWindowPointer());

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    ERROR_MSG("Failed to initialise GLAD!");
    return -1;
  }

  window.updateFramebuffer();

  UserInterface::get().init(window.getWindowPointer());

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

  SceneLoader::loadScene("..\\scenes\\cornellbox.scene", scene);

  renderLoop(&scene);

  terminate();

  return 0;
}

void Application::renderLoop(Scene* scene)
{
  Mesh* mesh = ResourceManager::get().getMesh("box"); // todo : move into scene->draw()

  scene->buildBVH();

  Shader debugShader{ "..\\shaders\\debug.vert", "..\\shaders\\debug.frag" };
  Shader offscreenShader{ "..\\shaders\\vs.vert", "..\\shaders\\fs.frag" };
  Shader compositionShader{ "..\\shaders\\composition.vert", "..\\shaders\\composition.frag" };
      
  Image rayTracedData{ window.getWidth(), window.getHeight(), 3 };
  Texture* rayTracedTexture = ResourceManager::get().addTexture("ray traced output", new Texture{ &rayTracedData, GL_RGB });

  Camera camera{ { 0.0f, 0.0f, 0.0f }, 1.0f, 45.0f, 0.1f, 2000.0f };
  window.setMainCamera(&camera);

  Matrix4 PV; // projection * view
  
  glfwSetWindowUserPointer(window.getWindowPointer(), &window);

  F32 deltaTime = 0.0f, previous = 0.0f;

  while (!glfwWindowShouldClose(window.getWindowPointer()))
  {
    F32 current = (F32)glfwGetTime();
    deltaTime = current - previous;
    previous = current;

    // INPUT ------------

    if (UserInterface::processKeyInput(&window, deltaTime) == 0)
    {
      raytracer.raytrace(scene, rayTracedData, window.getCamera(), antiAliasingEnabled);
      rayTracedData.writeToImageFile("..\\screenshots\\out.jpg");
      rayTracedTexture->generate(true); // also load image as a texture and display
    }

    UserInterface::get().set(this);

    // RENDER SCENE ------------

    window.framebuffer.bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.6f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    PV = camera.getProjectionViewMatrix();
    
    if (drawBVH)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      debugShader.use();
      debugShader.setMatrix4("PV", PV);
      scene->bvh->draw();
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    offscreenShader.use();
    offscreenShader.setMatrix4("PV", PV);
    if (mesh) mesh->draw(); // TODO: move into scene->draw()

    // RENDER GUI ------------
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    UserInterface::get().draw();

    glfwSwapBuffers(window.getWindowPointer());
    glfwPollEvents();
  }
}

void Application::terminate()
{
  UserInterface::get().terminate();

  glfwDestroyWindow(window.getWindowPointer());

  glfwTerminate();
}