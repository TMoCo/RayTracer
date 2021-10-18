#include <app/Application.h>

#include <core/debug.h>

#include <render/Shader.h>
#include <render/Texture.h>
#include <render/Mesh.h>
#include <render/bounds/AABB.h>

#include <resource/TextureLoader.h>
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

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  if (window.createWindow(800, 600))
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

int Application::run()
{

  if (init() != 0)
  {
    DEBUG_PRINT("Could not initialise app\n");
    return -1;
  }

  // main loop
  renderLoopGl();

  terminate();

  return 0;
}

void Application::renderLoopGl()
{
  // load mesh
  std::vector<Mesh*> meshes;
  OBJLoader::loadObj("C:\\Users\\Tommy\\Documents\\Graphics\\teapot.obj", meshes);
  for (auto& mesh : meshes)
    mesh->generateBuffers(false);
  
  // texture
  Texture containerTexture{};
  TextureLoader::loadTexture("C:\\Users\\Tommy\\Documents\\Graphics\\Textures\\container.jpg", containerTexture, GL_RGB);
  
  // shaders
  Shader shader{};
  shader.create(
    "C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\vs.vert",
    "C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\fs.frag");

  Shader debugShader{};
  debugShader.create(
    "C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\debug.vert",
    "C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\debug.frag");

  // AABB
  AABB aabb = AABB::getAABB(*meshes.front());
  aabb.generateBuffers();

  Transform t{ Vector3{0.0f, 0.0f, 0.0f}, Quaternion::angleAxis(RADIANS(0.0f), RIGHT) };

  Matrix4 model = t.toWorldMatrix();
  Matrix4 view;
  Matrix4 proj  = Matrix4::perspective(RADIANS(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
  
  const F32 radius = 10.0f;
  F32 camX, camZ;

  glEnable(GL_DEPTH_TEST);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  while (!glfwWindowShouldClose(window.ptr))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camX = sinf(static_cast<F32>(glfwGetTime())) * radius;
    camZ = cosf(static_cast<F32>(glfwGetTime())) * radius;

    view = Matrix4::lookAt(Vector3{ camX, 0.0f, camZ }, Vector3{ 0.0f, 0.0f, 0.0f }, UP);
    Matrix4 mvp = proj * view * model;


    // ... render
    {
      shader.use();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      shader.setMatrix4("transform", proj * view * model);
      containerTexture.bind(0);
      for (auto& mesh : meshes)
        mesh->draw();
    }

    {
      debugShader.use();
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      debugShader.setMatrix4("transform", proj * view * model);
      aabb.draw();
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