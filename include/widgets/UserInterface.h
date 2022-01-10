/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 26/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// UserInterface class declaration
//

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <app/Application.h>
#include <render/raytracer/RayTracer.h>

class UserInterface
{
  UserInterface();
public:
  static UserInterface& get();

  static int processKeyInput(Window* window, float deltaTime);

  static void mouseCallBack(GLFWwindow* glfwwindow, double x, double y);

  void init(GLFWwindow* window);

  void set(Application* application, Scene* scene, rt::RayTracerSettings* settings, Image& rayTraced);

  void draw();

  void terminate();

private:
  bool cursorEnabled, viewNormals;
};

#endif // !USER_INTERFACE_H



