#include <app/Application.h>

#include <core/debug.h>

#include <render/Shader.h>
#include <render/Texture.h>
#include <render/Mesh.h>
#include <render/bounds/AABB.h>
#include <render/bounds/KDOP.h>
#include <render/primitives/Quad.h>

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
  // shaders
  Shader shader{};
  shader.create(
    "C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\vs.vert",
    "C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\fs.frag");

  Shader debugShader{};
  debugShader.create(
    "C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\debug.vert",
    "C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\debug.frag");

  //exit(1);

  // load mesh
  std::vector<Mesh*> meshes;
  OBJLoader::loadObj("C:\\Users\\Tommy\\Documents\\Graphics\\teapot.obj", meshes);
  for (auto& mesh : meshes)
    mesh->generateBuffers(false);

  Quad quad = Quad::createQuad();
  
  // texture
  Texture containerTexture{};
  TextureLoader::loadTexture("C:\\Users\\Tommy\\Documents\\Graphics\\Textures\\container.jpg", containerTexture, GL_RGB);
  

  // AABB
  AABB aabb = AABB::getAABB(*meshes.front());
  aabb.generateBuffers();

  // k-DOP
  KDOP kdop = KDOP::getKDOP(*meshes.front(), KDOP::BOX | KDOP::EGDES | KDOP::CORNERS); // should be equivalent to the AABB
  // for viewing slabs
  std::vector<Transform> kdopData(kdop.slabs.size() * 2);
  for (UI64 i = 0; i < kdop.slabs.size(); ++i)
  {
    // position the quad along the normal at the specified distance
    kdopData[i * 2].position = kdop.slabs[i].normal * kdop.slabs[i].dmin;
    // rotate the quad to point in the slabs's direction
    kdopData[i * 2].orientation = Quaternion::getRotationFrom(UP, kdop.slabs[i].normal);
    // ditto for dmax
    kdopData[i * 2 + 1].position = kdop.slabs[i].normal * kdop.slabs[i].dmax;
    // rotate the quad to point in the slabs's direction
    kdopData[i * 2 + 1].orientation = Quaternion::getRotationFrom(UP, kdop.slabs[i].normal);
  }

  Transform t{ Vector3{0.0f, 0.0f, 0.0f}, Quaternion::angleAxis(RADIANS(55.0f), RIGHT) };

  Matrix4 model = Matrix4::identity(); // t.toWorldMatrix();
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

    // ... render
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      shader.use();
      model = Matrix4::identity();
      shader.setMatrix4("transform", proj * view * model);
      containerTexture.bind(0);
      for (auto& mesh : meshes)
        mesh->draw();
    }

    if (glfwGetKey(window.ptr, GLFW_KEY_I))
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      debugShader.use();
      // loop over each kdop slab and draw quads
      for (auto& t : kdopData)
      {
        model = t.toWorldMatrix();
        debugShader.setMatrix4("transform", proj * view * model);
        quad.draw();
      }

    }
    else
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      model = Matrix4::identity();
      debugShader.use();
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