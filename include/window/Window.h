//
// Window class definition
//

#ifndef WINDOW_H
#define WINDOW_H 1

#include <model/Model.h>
#include <window/OpenGLWidget.h>

#include <QtWidgets>

class Window : public QWidget{
private:
    OpenGLWidget* glWidget;

    QVBoxLayout* layout;
    QTimer* timer;
public:
    Window(const Model& model, const char* name = "Window");
};

#endif // ! WINDOW_H