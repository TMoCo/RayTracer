######################################################################
# Automatically generated by qmake (3.1) Fri Sep 10 16:31:08 2021
######################################################################

QT += opengl
TEMPLATE = app
TARGET = RayTracer
INCLUDEPATH += ./include/

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

MAKEFILE += ./build/Makefile


# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS +=  include/core/core.h \
            include/core/types.h \
            include/image/Image.h \
            include/math/Vector2.h \
            include/math/Vector3.h \
            include/math/Vector4.h \
            include/math/Quaternion.h \
            include/math/Matrix4.h \
            include/math/Transform.h \
            include/model/Model.h \
            include/model/OBJLoader.h \
            include/model/Primitives.h \
            include/window/ArcBall.h \
            include/window/RenderWidgets.h \
            include/window/RenderOptionsWidget.h \
            include/window/OpenGLWidget.h \
            include/window/RayTracerWidget.h \
            include/window/Window.h \
            include/raytracer/RayTracer.h \
            include/test.h

SOURCES +=  src/main.cpp \
            src/math/Vector2.cpp \
            src/math/Vector3.cpp \
            src/math/Vector4.cpp \
            src/math/Quaternion.cpp \
            src/math/Matrix4.cpp \
            src/math/Transform.cpp \
            src/model/Model.cpp \
            src/model/OBJLoader.cpp \
            src/model/Primitives.cpp \
            src/window/ArcBall.cpp \
            src/window/RenderWidgets.cpp \
            src/window/RenderOptionsWidget.cpp \
            src/window/OpenGLWidget.cpp \
            src/window/RayTracerWidget.cpp \
            src/raytracer/RayTracer.cpp \
            src/window/Window.cpp
