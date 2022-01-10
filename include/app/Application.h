/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

#ifndef APP_H
#define APP_H 1

#include <glad/glad.h>

#include <widgets/Window.h> 
#include <scene/Scene.h>

class Application 
{
  friend class UserInterface;

public:
  Application();

  ~Application();

  int run(int argc, char* argv[]);

private:
  void loop(Scene* scene);

protected:
  Window window;

  bool drawBVH;
};

#endif // !APP_H
