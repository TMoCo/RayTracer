//
// main
//

#include <app/Application.h>

int main(int argc, char *argv[])
{
  /*
  Transform t{ Vector3{0.0f, 0.0f, 0.0f}, Quaternion::angleAxis(RIGHT, RADIANS(-55.0f)) };

  Matrix4 m;
  for (UI32 i = 0; i < 16; ++i)
    m[i >> 2][i & 3] = i;

  std::cout << m << "\n";

  std::cout << "col1: " << Vector4{ m[0] } << "\n";
  std::cout << "col2: " << Vector4{ m[1] } << "\n";
  std::cout << "col3: " << Vector4{ m[2] } << "\n";
  std::cout << "col4: " << Vector4{ m[3] } << "\n\n";

  std::cout << m * m << "\n\n\n";


  Matrix4 model = t.toWorldMatrix();
  Matrix4 view = Matrix4::TranslationMatrix(Vector3{ 0.0f, 0.0f, -3.0f });
  Matrix4 proj = Matrix4::Perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

  std::cout << "model\n";
  std::cout << "col1: " << Vector4{ model[0] } << "\n";
  std::cout << "col2: " << Vector4{ model[1] } << "\n";
  std::cout << "col3: " << Vector4{ model[2] } << "\n";
  std::cout << "col4: " << Vector4{ model[3] } << "\n\n";

  std::cout << "modelview\n" << view * model << "\n\n";
  std::cout << "view\n" << view << "\n\n";
  std::cout << "proj\n" << proj << "\n\n";

  Matrix4 mvp = proj * view * model;

  std::cout << "mvp\n" << mvp << "\n\n";

  std::cout << Vector4{ mvp[0] } << "\n";
  std::cout << Vector4{ mvp[1] } << "\n";
  std::cout << Vector4{ mvp[2] } << "\n";
  std::cout << Vector4{ mvp[3] } << "\n";

  return 1;
  */

  Application app{};

  return app.run();
}
