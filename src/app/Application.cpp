#include <app/Application.h>

#include <core/debug.h>

#include <render/Shader.h>

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
  // startup render data
  F32 vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
  };

  UI32 indices[] = {  // note that we start from 0!
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(F32), (void*)0);
  glEnableVertexAttribArray(0);

  Shader shader{};

  // shaders
  shader.create("C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\vs.vert",
    "C:\\Users\\Tommy\\Documents\\Graphics\\RayTracer\\src\\shaders\\fs.frag");

  shader.use();

  glBindVertexArray(vao);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  while (!glfwWindowShouldClose(window.ptr))
  {
    glClear(GL_COLOR_BUFFER_BIT);

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