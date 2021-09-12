//
// Window class definition
//

#ifndef WINDOW_H
#define WINDOW_H 1

#include <model/Model.h>

#include <QtWidgets>

class Window : public QWidget{
private: 
    Model model;

public:
    Window(QWidget* parent);
};

#endif // ! WINDOW_H