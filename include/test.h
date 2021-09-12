#include <math/Matrix4.h>

void TestMatrix() {

    float values[16] {};

    for (uint32_t v = 0; v < 16; ++v) {
        values[v] = v;
    }

    Matrix4 m {values};
    Matrix4 n = m;

    if (n == m) {
        std::cout << "equal\n";
    }
    if (!((n*3) == m)) {
        std::cout << "not equal\n";
    }

    std::cout << "transpose:\n" << n.Transpose() << '\n';

    std::cout << "add:\n" << n + m << '\n';
    std::cout << "sub:\n" << n - m << '\n';
    std::cout << "prod:\n" << n * m << '\n';
    std::cout << "div:\n" << n / 2.0f << '\n';
    std::cout << "fac:\n" << n * 2.0f << '\n';
}


void Test(char c) {
    switch(c) {
        case 'm':
            TestMatrix();
    }
}
