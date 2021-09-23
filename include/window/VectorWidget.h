//
// Widget for editing a vector's fields
//

#ifndef VECTOR_WIDGET_H
#define VECTOR_WIDGET_H 1

#include <math/Vectors.h>

#include <qt5/QtWidgets/QtWidgets>

class VectorWidget : public QWidget {
Q_OBJECT
private:
    Vector3* vector;

    // spin boxes for editing vector fields
    QDoubleSpinBox* xSpinBox;
    QDoubleSpinBox* ySpinBox;
    QDoubleSpinBox* zSpinBox;

    QHBoxLayout* layout;

public:
    VectorWidget(Vector3* v, F32 min, F32 max, QWidget* parent);
    ~VectorWidget();

public slots:
    void UpdateX(double X);
    void UpdateY(double Y);
    void UpdateZ(double Z);

signals:
    void ValueChanged();
};

#endif // ! VECTOR_WIDGET_H