#include <app/Application.h>

#include <core/debug.h>

#include <render/Shader.h>
#include <render/Texture.h>
#include <render/Mesh.h>

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
  
  // load mesh
  std::vector<Mesh*> meshes;
  OBJLoader::loadObj("C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\models\\Triangle.obj", meshes);

  std::cout << meshes.front()->indices.size() << "\n";
  std::cout << meshes.front()->positions.size() << "\n";
  std::cout << meshes.front()->normals.size() << "\n";
  std::cout << meshes.front()->textureCoords .size() << "\n";

  // main loop
  renderLoopGl();

  terminate();

  return 0;
}

void Application::renderLoopGl()
{
  // startup render data
  F32 vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
  };

  UI32 indices[] = { 
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
  };

  // vertex attributes
  UI32 vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // vertex buffer
  UI32 vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // indices
  UI32 ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(F32), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(F32), (void*)(3 * sizeof(F32)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(F32), (void*)(6 * sizeof(F32)));
  glEnableVertexAttribArray(2);

  // texture
  std::string path = "C:\\Users\\Tommy\\Documents\\Graphics\\Textures\\container.jpg";
  Texture containerTexture{};
  TextureLoader::loadTexture(path, containerTexture, GL_RGB);
  
  // shaders
  Shader shader{};
  shader.create("C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\vs.vert",
    "C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\fs.frag");

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  Transform t{ Vector3{0.0f, 0.0f, 0.0f}, Quaternion::angleAxis(RADIANS(-55.0f), RIGHT) };

  Matrix4 model = t.toWorldMatrix();
  Matrix4 view{};// = Matrix4::translation(Vector3{ 0.0f, 0.0f, -3.0f });
  Matrix4 proj  = Matrix4::perspective(RADIANS(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
  
  view = Matrix4::lookAt(Vector3{ 0.0f, 0.0f, 3.0f}, Vector3{ 0.0f, 0.0f, 0.0f }, UP);
  
  shader.use();
  containerTexture.bind();
  glBindVertexArray(vao);

  const F32 radius = 10.0f;
  F32 camX, camZ;
  while (!glfwWindowShouldClose(window.ptr))
  {
    glClear(GL_COLOR_BUFFER_BIT);

    camX = sinf(static_cast<F32>(glfwGetTime())) * radius;
    camZ = cosf(static_cast<F32>(glfwGetTime())) * radius;

    view = Matrix4::lookAt(Vector3{ camX, 0.0f, camZ }, Vector3{ 0.0f, 0.0f, 0.0f }, UP);
    Matrix4 mvp = proj * view * model;
    shader.setMatrix4("transform", proj * view * model);
    // ... render
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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