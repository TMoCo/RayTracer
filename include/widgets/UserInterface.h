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

class UserInterface
{
  UserInterface();
public:
  static UserInterface& get();

  static I32 processKeyInput(Window* window, F32 deltaTime);

  static void mouseCallBack(GLFWwindow* glfwwindow, F64 x, F64 y);

  void init(GLFWwindow* window);

  void set(Application* application);

  void draw();

  void terminate();

private:
  bool cursorEnabled, viewNormals;
};

#endif // !USER_INTERFACE_H



