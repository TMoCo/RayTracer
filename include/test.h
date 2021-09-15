#include <math/Matrix4.h>
#include <math/Quaternion.h>

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

    std::cout << m[0][0] << ' ' << m[0][1] << ' ' << m[0][2] << ' ' << m[0][3] << '\n';
    std::cout << m[1][0] << ' ' << m[1][1] << ' ' << m[1][2] << ' ' << m[1][3] << '\n';

    std::cout << "transpose:\n" << Matrix4::Transpose(m) << '\n';

    std::cout << "add:\n" << n + m << '\n';
    std::cout << "sub:\n" << n - m << '\n';
    std::cout << "prod:\n" << n * m << '\n';
    std::cout << "m:\n"<< m << '\n';
    std::cout << "n:\n"<< n << '\n';
    std::cout << "div:\n" << n / 2.0f << '\n';
    std::cout << "fac:\n" << n * 2.0f << std::endl;
}

void TestQuaternion() {

    Quaternion q{0.5f, 0.0f, 0.5f, 0.5f};
    Quaternion p = Quaternion::AngleAxis({0.0f, 1.0f, 0.0f}, 30.0f);

    if (p == q) {
        std::cout << "equal\n";
    }
    else {
        std::cout << "not equal\n";
    }

    std::cout << "quaternion\n" << p << '\n';
    std::cout << "matrix\n" << Matrix4::RotationMatrix(p) << '\n';

    std::cout << "norm:\n" << q.Norm() << '\n';
    std::cout << "inverse:\n" << q.Inverse() << '\n';
    std::cout << "conjugate:\n" << q.Conjugate() << '\n';
    std::cout << "unit:\n" << q.Unit() << '\n';

    std::cout << "prod:\n" << p * q << '\n';
}

void Test(const char* test) {
    if (strcmp(test, "matrix") == 0)
        TestMatrix();
    if (strcmp(test, "quat") == 0)
        TestQuaternion();
}
