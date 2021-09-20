//
// main
//

#include <math/Vector3.h>
#include <math/Matrix4.h>
#include <model/OBJLoader.h>
#include <window/Window.h>
#include <test.h>

#include <chrono>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: ./RayTracer ./path/to/model.obj" << std::endl;
        return 1;
    }

    // test mode
    if (strcmp(argv[1], "test") == 0) {
        if (argc != 3) {
            std::cerr << "No test function selected\n";
            return 1;
        }
        Test(argv[2]);
        return 0;
    }

    QApplication app(argc, argv);

    Model model;
    OBJLoader::LoadObj(argv[1], model); // load model from obj

    Window window{&model, "RayTracing"}; 
    window.resize(900, 700);
    window.show();

   return app.exec();
}
