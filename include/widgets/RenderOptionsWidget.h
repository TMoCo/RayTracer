//
// Render options widget class declaration
//

#ifndef RENDER_OPTIONS_WIDGETS_H
#define RENDER_OPTIONS_WIDGETS_H 1

#include <render/Camera.h>

#include <math/Transform.h>

#include <window/VectorWidget.h>

#include <qt5/QtWidgets/QtWidgets>

class RenderOptionsWidget : public QWidget {
Q_OBJECT
private:
    QVBoxLayout* layout;

    Camera* camera;

    // camera options
    QLabel* cameraOptionsLabel;
    QGroupBox* cameraGroupBox;
    QVBoxLayout* cameraBoxLayout;
    QLabel* fovLabel;
    QDoubleSpinBox* fovSpinBox;
    QLabel* aspectRatioLabel;
    QCheckBox* windowAspectRatio;
    QDoubleSpinBox* aspectRatioSpinBox;

    // model options
    QLabel* translationLabel;
    QGroupBox* translationGroupBox;
    QVBoxLayout* translationBoxLayout;
    VectorWidget* positionWidget;

    // raytracing options
    QLabel* rayTracingOptionsLabel;
    QGroupBox* rayTracingGroupBox;
    QVBoxLayout* rayTracingBoxLayout;
    QLabel* samplesLabel;
    QSpinBox* samplesSpinBox;
    QPushButton* rayTraceButton; // show / disable on render type
    QPushButton* saveImageButton;

public:
    RenderOptionsWidget(QWidget* parent, Transform* transform, Camera* camera);
    ~RenderOptionsWidget();

public slots:
    void SwitchRender(int renderIndex);  
    void ShowAspectRatio(int state); 
    void UpdateProperties(); 
    void PressedRayTrace();
    void PressedSaveImage();

signals:
    void ShouldResetAspectRatio();
    void ShouldUpdateGl();
    void ShouldRayTrace(int samples);
    void ShouldSaveImage(QString name);
};

#endif