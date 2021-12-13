/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#include <app/Application.h>

#include <core/debug.h>

#include <render/Shader.h>
#include <render/Texture.h>
#include <render/bounds/AABB.h>
#include <render/bounds/KDOP.h>

#include <resource/TextureLoader.h>
#include <resource/SceneLoader.h>
#include <resource/OBJLoader.h>

#include <string>
#include <iomanip>

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
  SceneLoader::loadScene("C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\scenes\\sphere.scene", scene);

  renderLoopGl(&scene);

  terminate();

  return 0;
}

void Application::renderLoopGl(Scene* scene)
{
  //Mesh* triangles = resourceManager.tryGetMesh("C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\models\\Triangles.obj");
  //triangles->generatebuffers(false);

  // shaders for scene
  //Shader shader{};
  //shader.create(
    //"C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\vs.vert",
    //"C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\fs.frag");

  //Shader debugShader{}; // for viewing AABB, view frustum, BVH hierarchies
  //debugShader.create(
    //"C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\debug.vert",
    //"C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\debug.frag");

  // load meshes from obj (eg, build a scene in blender)
  // Mesh* teapot = resourceManager.getMesh("C:\\Users\\Tommy\\Documents\\Graphics\\teapot.obj");
  // teapot->generatebuffers(false);

  buffer<colour> imageBuffer{ window.getWidth(), window.getHeight() };

  // Texture containerTexture{};
  // TextureLoader::loadTexture("C:\\Users\\Tommy\\Documents\\Graphics\\Textures\\container.jpg", containerTexture, GL_RGB);
  
  Camera camera{ { 0.0f, 0.0f, 2.0f } };

  window.setMainCamera(&camera);

  Transform t{ Vector3{ 1.0f, 2.0f, 3.0f }, Quaternion::angleAxis(radians(90.0f), RIGHT), {1.0f, 1.0f, 1.0f } };
  
  Matrix4 model = t.matrix;
  Matrix4 view;
  Matrix4 proj;
  
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  glfwSetWindowUserPointer(window.getWindowPointer(), &window);

  F32 deltaTime = 0.0f, previous = 0.0f;
  while (!glfwWindowShouldClose(window.getWindowPointer()))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    F32 current = static_cast<F32>(glfwGetTime());
    deltaTime = current - previous;
    previous = current;

    if (!Window::processInput(&window, deltaTime, pause)) break;

    if (glfwGetKey(window.getWindowPointer(), GLFW_KEY_R))
    {
      // ... raytrace scene
      raytracer.raytrace(imageBuffer, scene, window.getCamera(), 1);
      TextureLoader::writeTexture("../screenshots/out.jpg", imageBuffer);
    }

    // ... render scene
    view = camera.getViewMatrix();
    proj = Matrix4::perspective(radians(camera.FOV), camera.aspectRatio, camera.zNear, camera.zFar);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //shader.use();
    //shader.setMatrix4("transform", proj * camera.getViewMatrix() * model);
    //containerTexture.bind(0);


    // ... debug
    if (debug)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      //debugShader.use();
      //debugShader.setMatrix4("transform", proj * camera.getViewMatrix() * model);
    }

    glfwPollEvents();

#ifdef OPENGL
    glfwSwapBuffers(window.getWindowPointer());
#endif
  }
}

void Application::renderLoopVk()
{
  // do something
}

void Application::terminate()
{
  glfwTerminate();
}