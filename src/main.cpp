//
// main
//

#include <math/Vector3.h>
#include <math/Matrix4.h>
#include <model/OBJLoader.h>
#include <test.h>

#include <chrono>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <unistd.h>

int main(int argc, char const *argv[])
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
        Test(argv[2][0]);
        return 0;
    }
    Model model;
    
    OBJLoader::LoadObj(argv[1], model);

    return 0;
}
