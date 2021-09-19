//
// Window class definition
//

#ifndef WINDOW_H
#define WINDOW_H 1

#include <model/Model.h>
#include <window/RenderWidgets.h>
#include <window/RenderOptionsWidget.h>

#include <QtWidgets>

class Window : public QWidget{
private:
    // main layout
    QGridLayout* layout;

    // render widgets
    RenderWidgets* renderWidgets;
    RenderOptionsWidget* renderOptionsWidget;

    // switch render widget
    QPushButton* switchButton;    

    Transform transform;
    Camera camera;

public:
    Window(Model* model, const char* name = "Window");
    ~Window();
};

#endif // ! WINDOW_H