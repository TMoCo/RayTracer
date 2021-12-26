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
public:
  static UserInterface& get();

  static I32 processKeyInput(Window* window, F32 deltaTime);

  void init(GLFWwindow* window);

  void set(Application* application);

  void draw();

  void terminate();
};

#endif // !USER_INTERFACE_H



