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
#include <resource/TextureLoader.h>
#include <resource/SceneLoader.h>

int Application::init()
{
  if (!glfwInit())
  {
    DEBUG_PRINT("Failed to initialise GLFW!\n");
    return -1;
  }
  debug = pause = false;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (window.create(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Ray Tracer Window"))
  {
    DEBUG_PRINT("Failed to create window!\n");
    return -1;
  }

  glfwMakeContextCurrent(window.getWindowPointer());

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    DEBUG_PRINT("Failed to initialize GLAD!\n");
    return -1;
  }

  window.setViewPort();

  return 0;
}

I32 Application::run()
{
  if (init() != 0)
  {
    DEBUG_PRINT("Failed to initialise app!\n");
    return -1;
  }

  Scene scene;

  SceneLoader::loadScene("..\\scenes\\sphere.scene", scene);

  raytracer.setScene(&scene);

  renderLoop(&scene);

  terminate();

  return 0;
}

void Application::renderLoop(Scene* scene)
{  
  // test textures
  /*
  Texture containerTexture;
  TextureLoader::loadTextureFromImageFile("C:\\Users\\Tommy\\Documents\\Graphics\\Textures\\container.jpg", containerTexture, GL_RGB);
  Texture earthMapTexture;
  TextureLoader::loadTextureFromImageFile("C:\\Users\\Tommy\\Documents\\Graphics\\Textures\\earthmap.jpg", earthMapTexture, GL_RGB);
  */

  OBJLoader::loadObj("..\\..\\models\\triangle.obj", resourceManager, true);
  const Mesh* mesh = resourceManager.getMesh("triangle");

  // test materials
  Metal metal = Metal({ 0.8f, 0.8f, 0.8f }, 0.2f);
  Dielectric glass = Dielectric(1.5f);
  Lambertian floor = Lambertian({ 0.8f, 0.8f, 0.0f });
  Lambertian mauve = Lambertian({ 0.3f, 0.1f, 0.6f });
  // TexturedLambertian earthMap = TexturedLambertian(&earthMapTexture);
  DiffuseLight light = DiffuseLight({ 4.0f, 4.0f, 4.0f });
  
  // set material
  auto primitives = *scene->getPrimitives();
  ((GeometricPrimitive*)primitives[0])->material = &glass;
  ((GeometricPrimitive*)primitives[1])->material = &light;
  ((GeometricPrimitive*)primitives[2])->material = &floor;

  BVH bvh = BVH(scene);

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
      raytracer.raytrace(rayTracedImage, window.getCamera(), 20);
      rayTracedImage.writeToImageFile("..\\screenshots\\out.jpg");
    }

    // ... draw a mesh
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    testShader.use();
    testShader.setMatrix4("transform", PV);
    mesh->draw();
    
    // ... debug
    if (debug)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      debugShader.use();
      debugShader.setMatrix4("VP", PV);
      bvh.draw();
    }

    glfwPollEvents();
    glfwSwapBuffers(window.getWindowPointer());
  }
}

void Application::terminate()
{
  glfwTerminate();
}