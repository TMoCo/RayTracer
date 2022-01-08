/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// main
//

#include <app/Application.h>
#ifndef NDEBUG
#include <test.h>
#endif // !NDEBUG


int main(int argc, char* argv[])
{
  Application app;

#ifndef NDEBUG
  return test::launch(0);
#else
  return app.run(argc, argv);
#endif // !NDEBUG
}
