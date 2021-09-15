//
// Window class definition
//

#ifndef WINDOW_H
#define WINDOW_H 1

#include <model/Model.h>
#include <window/RenderWidgets.h>

#include <QtWidgets>

class Window : public QWidget{
private:
    // main layout
    QVBoxLayout* layout;

    // render widgets
    RenderWidgets* renderWidgets;

    // switch render widget
    QPushButton* switchButton;    

public:
    Window(const Model& model, const char* name = "Window");
    ~Window();
};

#endif // ! WINDOW_H