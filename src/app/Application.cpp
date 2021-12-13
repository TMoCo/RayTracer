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

// app setup
int Application::init()
{
  // setup glfw 
  if (!glfwInit())
  {
    DEBUG_PRINT("Failed to initialise GLFW!\n");
    return -1;
  }

  debug = pause = false;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  if (window.create(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Ray Tracer Window"))
  {
    DEBUG_PRINT("Failed to create window!\n");
    return -1;
  }

  glfwMakeContextCurrent(window.getWindowPointer());

  // load opengl 
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    DEBUG_PRINT("Failed to initialize GLAD!\n");
    return -1;
  }

  return 0;
}

I32 Application::run()
{
  if (!init())
  {
    DEBUG_PRINT("Failed to initialise app!\n");
    return -1;
  }

  Scene* scene = nullptr;
  SceneLoader::loadScene("C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\scenes\\sphere.scene", scene);

  // main loop
  renderLoopGl(scene);

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

  // frame buffer
  buffer<colour> fb{ window.getWidth(), window.getHeight() };

  // texture (TODO: make part of material struct)
  // Texture containerTexture{};
  // TextureLoader::loadTexture("C:\\Users\\Tommy\\Documents\\Graphics\\Textures\\container.jpg", containerTexture, GL_RGB);
  
  // scene camera
  Camera camera{ { 0.0f, 0.0f, 2.0f } };

  window.setMainCamera(&camera);

  // global scene transform
  Transform t{ Vector3{ 1.0f, 2.0f, 3.0f }, Quaternion::angleAxis(radians(90.0f), RIGHT), {1.0f, 1.0f, 1.0f } };
  
  std::cout << t.transformVector3({ 1,2,3 }) << "\n";

  Matrix4 model = t.matrix;
  Matrix4 view; // update every frame
  Matrix4 proj;
  
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  glfwSetWindowUserPointer(window.getWindowPointer(), &window);

  F32 deltaTime = 0.0f;
  F32 previous = 0.0f;
  while (!glfwWindowShouldClose(window.getWindowPointer()))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    F32 current = static_cast<F32>(glfwGetTime());
    deltaTime = current - previous;
    previous = current;

    // ... process input
    if (Window::processInput(&window, deltaTime, pause) == 0) break;

    if (glfwGetKey(window.getWindowPointer(), GLFW_KEY_R))
    {
      // ... raytrace scene
      raytracer.raytrace(fb, scene, window.getCamera(), 1);
      TextureLoader::writeTexture("../screenshots/out.jpg", fb);
    }

    // ... render scene
    view = camera.getViewMatrix();
    proj = Matrix4::perspective(radians(camera.FOV), camera.aspectRatio, camera.zNear, camera.zFar);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //shader.use();
    //shader.setMatrix4("transform", proj * camera.getViewMatrix() * model);
    //containerTexture.bind(0);

    //triangles->draw();

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