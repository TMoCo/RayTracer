cmake_minimum_required (VERSION 3.20.5)

project(RAYTRACER
  VERSION 1.0
  LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# qt params
set(CMAKE_AUTOMOC ON)

# find source code
set(SOURCE
  src/main.cpp
  src/image/Image.cpp
  src/math/Matrix4.cpp
  src/math/Quaternion.cpp
  src/math/Transform.cpp
  src/math/Vector2.cpp
  src/math/Vector3.cpp
  src/math/Vector4.cpp
  src/model/Model.cpp
  src/model/OBJLoader.cpp
  src/model/Primitives.cpp
  src/raytracer/RayTracer.cpp
  src/widgets/ArcBall.cpp
  src/widgets/VectorWidget.cpp
  src/widgets/RenderOptionsWidget.cpp
  src/widgets/RenderWidget.cpp
  src/widgets/Window.cpp)

# include these for moc generation
set(QTHEADERS
  include/widgets/VectorWidget.h
  include/widgets/RenderOptionsWidget.h
  include/widgets/RenderWidget.h
  include/widgets/Window.h)

# create executable
add_executable(${PROJECT_NAME} ${SOURCE} ${QTHEADERS})

# Qt
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL REQUIRED)

# openGl
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)

add_dependencies(${PROJECT_NAME} OpenGL::OpenGL)

# include
include_directories(include ${GLUT_INCLUDE_DIRS})

# link libraries
target_link_libraries(${PROJECT_NAME}
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::OpenGL
  OpenGL::OpenGL
  GLUT::GLUT)
