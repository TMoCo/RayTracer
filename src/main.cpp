/*
* AUTHOR: THOMAS MOENO COOPER
* LAST MODIFIED: 13/12/2021
* COPYRIGHT UNDER THE MIT LICENSE
*/

//
// main
//

// #define RUN_TEST

#include <app/Application.h>

#ifdef RUN_TEST
#include <test.h>
#endif // !NDEBUG


int main(int argc, char* argv[])
{
  Application app;

#ifdef RUN_TEST
  return test::launch(0);
#else
  return app.run(argc, argv);
#endif // !NDEBUG
}
