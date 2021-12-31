/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// main
//

#include <app/Application.h>

int main(int argc, char* argv[])
{
  Application app;

  if (argc > 1)
  {
    return app.run(argv[1]);
  }
  else
  {
#ifdef NDEBUG
    ERROR_MSG("To use app:\n>RAYTRACER.exe scene\nMake sure .scene file is in the scenes folder");
    return -1;
#else
    return app.run("cornellbox.scene");
#endif // !NDEBUG

  }
}
