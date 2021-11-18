#include <app/Application.h>

#include <core/debug.h>

#include <render/Shader.h>
#include <render/Texture.h>
#include <render/Mesh.h>
#include <render/bounds/AABB.h>
#include <render/bounds/KDOP.h>
#include <render/raytracer/BVH.h>

#include <resource/TextureLoader.h>
#include <resource/SceneLoader.h>
#include <resource/OBJLoader.h>

#include <string>

// app setup
int Application::init()
{
  // setup glfw 
  if (!glfwInit())
  {
    DEBUG_PRINT("Could not initialise gltw\n");
    return -1;
  }

  debug = pause = false;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  if (window.createWindow(800, 600, "RayTracer window"))
    return -1;

  glfwMakeContextCurrent(window.ptr);

  // load opengl 
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    DEBUG_PRINT("Failed to initialize GLAD\n");
    return -1;
  }

  return 0;
}

I32 Application::run()
{

  if (init() != 0)
  {
    DEBUG_PRINT("Could not initialise app\n");
    return -1;
  }

  Scene* scene = new Scene;
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
  Transform t{ Vector3{ 0.0f, 0.0f, 0.0f }, Quaternion::angleAxis(radians(0.0f), RIGHT) };

  Matrix4 model = t.matrix;
  Matrix4 view; // update every frame
  Matrix4 proj;
  
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  glfwSetWindowUserPointer(window.ptr, &window);

  F32 deltaTime = 0.0f;
  F32 previous = 0.0f;
  while (!glfwWindowShouldClose(window.ptr))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    F32 current = static_cast<F32>(glfwGetTime());
    deltaTime = current - previous;
    previous = current;

    // ... process input
    if (Window::processInput(&window, deltaTime) == 0)
      break;

    if (glfwGetKey(window.ptr, GLFW_KEY_R))
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
    glfwSwapBuffers(window.ptr);
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