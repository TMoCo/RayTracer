//
// Vector widget class definition
//

/*
#include <window/VectorWidget.h>

VectorWidget::VectorWidget(Vector3* v, F32 min, F32 max, QWidget* parent) : 
    QWidget(parent), vector(v) {

    layout = new QHBoxLayout(this);
    
    xSpinBox = new QDoubleSpinBox(this);
    xSpinBox->setRange(min, max);
    xSpinBox->setValue(vector->x);
    xSpinBox->setPrefix("x: ");
    layout->addWidget(xSpinBox);
    
    ySpinBox = new QDoubleSpinBox(this); 
    ySpinBox->setRange(min, max);
    ySpinBox->setValue(vector->y);
    ySpinBox->setPrefix("y: ");
    layout->addWidget(ySpinBox);

    zSpinBox = new QDoubleSpinBox(this);
    zSpinBox->setRange(min, max);
    zSpinBox->setValue(vector->z);
    zSpinBox->setPrefix("z: ");
    layout->addWidget(zSpinBox);

    QObject::connect(xSpinBox, SIGNAL(valueChanged(double)), this, SLOT(UpdateX(double)));
    QObject::connect(ySpinBox, SIGNAL(valueChanged(double)), this, SLOT(UpdateY(double)));
    QObject::connect(zSpinBox, SIGNAL(valueChanged(double)), this, SLOT(UpdateZ(double)));
}

VectorWidget::~VectorWidget() {
    delete zSpinBox;
    delete ySpinBox;
    delete xSpinBox;

    delete layout;
}

void VectorWidget::UpdateX(double X) {
    vector->x = X;
    emit ValueChanged();
}

void VectorWidget::UpdateY(double Y) {
    vector->y = Y;
    emit ValueChanged();
}

void VectorWidget::UpdateZ(double Z) {
    vector->z = Z;
    emit ValueChanged();
}
*/