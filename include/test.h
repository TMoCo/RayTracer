#include <math/Matrix4.h>
#include <math/Quaternion.h>
#include <math/Transform.h>
#include <image/Image.h>

#include <chrono>
#include <iomanip>


void TestMatrix() {

    float values0[16] {};

    for (uint32_t v = 0; v < 16; ++v) {
        values0[v] = v;
    }

    Matrix4 m {values0};
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

    Transform t{{0.0f, 0.0f, 5.0f}, {}};

    Vector3 p{0, 0, 0};
    Vector3 newP = t.TransformPoint(p);

    std::cout << newP << "\n";

    /*
    float values1[16] = {2, 3, -4, 0, 11, 8, 7, 0, 2, 5, 3, 0, 0, 0, 0, 1};
    Matrix4 o{values1};
    o = Matrix4::Identity();


    UI32 i;
    auto s = std::chrono::high_resolution_clock().now();
    for (i = 0; i < 1000000000; ++i)
        o * vec0;
    auto t = std::chrono::high_resolution_clock().now();
    std::cout << "vec: " << o * vec0 << "  " << std::chrono::duration<float>(t - s).count() << std::endl;
    */
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

void TestImage() {
    rgba_f a = {0.0F, 10.0f, 100.0f, 200.0f };
    Vector4 v{a.channels}; // rgba to a vector
    std::cout << v << '\n' << sizeof(a) << std::endl;
}

void TestVec() {
    Vector4 v4_0 = {};
    Vector4 v4_1{1.0F, 1.0F, 1.0F, 1.0F};
    Vector3 v3_0 = {};
    Vector3 v3_1 = {1.0F, 1.0F, 1.0F};
    uint32_t i;
    auto s = std::chrono::high_resolution_clock().now();
    for (i = 0; i < 1000000000; ++i)
        v4_0 += v4_1;
    auto t = std::chrono::high_resolution_clock().now();
    std::cout << v4_0  << std::setprecision(10)  << " intrinsics:    " << std::chrono::duration<float>(t - s).count() << '\n';
    s = std::chrono::high_resolution_clock().now();
    for (i = 0; i < 1000000000; ++i)
        v3_0 += v3_1;
    t = std::chrono::high_resolution_clock().now();
    std::cout << v3_0  << std::setprecision(10) << " not intrinsic: " << std::chrono::duration<float>(t - s).count() << '\n';
}

void Test(const char* test) {
    if (strcmp(test, "matrix") == 0)
        TestMatrix();
    if (strcmp(test, "quat") == 0)
        TestQuaternion();
    if (strcmp(test, "image") == 0)
        TestImage();
    if (strcmp(test, "vec") == 0)
        TestVec();
}
