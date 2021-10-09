//
// Render options widget class definition
//

#include <window/RenderOptionsWidget.h>

RenderOptionsWidget::RenderOptionsWidget(QWidget* parent, Transform* t, Camera* c) 
    : QWidget(parent), camera(c) {

    setMinimumWidth(250);

    // layout
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QString groupBoxStyleSheet{
        "QGroupBox {  border: 1px solid lightgray; font-weight: bold; }"};

    QString titleStyleSheet{"QLabel { font-weight: bold; }"};

    // > camera editing

    cameraOptionsLabel = new QLabel("Camera options", this);
    cameraOptionsLabel->setStyleSheet(titleStyleSheet);
    layout->addWidget(cameraOptionsLabel);

    cameraGroupBox = new QGroupBox(this);
    cameraGroupBox->setStyleSheet(groupBoxStyleSheet);
    cameraBoxLayout = new QVBoxLayout(cameraGroupBox);
    
    fovLabel = new QLabel("FOV", this);
    cameraBoxLayout->addWidget(fovLabel);
    
    fovSpinBox = new QDoubleSpinBox(nullptr);
    fovSpinBox->setSuffix("°");
    fovSpinBox->setRange(0.0, 180.0);
    fovSpinBox->setValue(camera->FOV);
    cameraBoxLayout->addWidget(fovSpinBox);

    QObject::connect(
        fovSpinBox, SIGNAL(valueChanged(double)), 
        this, SLOT(UpdateProperties()));

    aspectRatioLabel = new QLabel("Aspect Ratio", this);
    cameraBoxLayout->addWidget(aspectRatioLabel);

    windowAspectRatio = new QCheckBox("Use window dimensions", this);
    windowAspectRatio->setCheckState(Qt::CheckState::Checked);
    cameraBoxLayout->addWidget(windowAspectRatio);
    QObject::connect(
        windowAspectRatio, SIGNAL(stateChanged(int)), 
        this, SLOT(ShowAspectRatio(int)));
    
    aspectRatioSpinBox = new QDoubleSpinBox(this);
    cameraBoxLayout->addWidget(aspectRatioSpinBox);
    aspectRatioSpinBox->setRange(0.0, 4.0);
    aspectRatioSpinBox->setSingleStep(0.05f);
    aspectRatioSpinBox->setValue(camera->aspectRatio);
    aspectRatioSpinBox->setVisible(false); // start not visible

    QObject::connect(
        aspectRatioSpinBox, SIGNAL(valueChanged(double)), 
        this, SLOT(UpdateProperties()));

    layout->addWidget(cameraGroupBox);

    // > transform editing

    translationLabel = new QLabel("Translation options", this);
    translationLabel->setStyleSheet(titleStyleSheet);
    layout->addWidget(translationLabel);

    translationGroupBox = new QGroupBox(this);
    translationGroupBox->setStyleSheet(groupBoxStyleSheet);

    translationBoxLayout = new QVBoxLayout(translationGroupBox);

    positionWidget = new VectorWidget(&t->position, -10.0f, 10.0f, this);
    translationBoxLayout->addWidget(positionWidget);
    QObject::connect(
        positionWidget, SIGNAL(ValueChanged()), 
        this, SLOT(UpdateProperties()));

    layout->addWidget(translationGroupBox);

    // > raytracing
    rayTracingOptionsLabel = new QLabel("RayTracing options");
    rayTracingOptionsLabel->setStyleSheet(titleStyleSheet);
    rayTracingOptionsLabel->setVisible(false);
    layout->addWidget(rayTracingOptionsLabel);

    rayTracingGroupBox = new QGroupBox(this);
    rayTracingGroupBox->setStyleSheet(groupBoxStyleSheet);
    rayTracingGroupBox->setVisible(false);

    rayTracingBoxLayout = new QVBoxLayout(rayTracingGroupBox);

    samplesLabel = new QLabel("Samples");
    rayTracingBoxLayout->addWidget(samplesLabel);

    samplesSpinBox = new QSpinBox(this);
    samplesSpinBox->setRange(1, 100);
    rayTracingBoxLayout->addWidget(samplesSpinBox);

    rayTraceButton = new QPushButton("RayTrace Image", this);
    rayTracingBoxLayout->addWidget(rayTraceButton);

    QObject::connect(
        rayTraceButton, SIGNAL(pressed()), 
        this, SLOT(PressedRayTrace()));

    saveImageButton = new QPushButton("Save Image", this);
    rayTracingBoxLayout->addWidget(saveImageButton);
    QObject::connect(
        saveImageButton, SIGNAL(pressed()), 
        this, SLOT(PressedSaveImage()));


    layout->addWidget(rayTracingGroupBox);
}

RenderOptionsWidget::~RenderOptionsWidget() {
    delete saveImageButton;
    delete rayTraceButton;
    
    delete positionWidget;
    delete translationBoxLayout;
    delete translationGroupBox;
    delete translationLabel;

    delete aspectRatioSpinBox;
    delete windowAspectRatio;
    delete aspectRatioLabel;
    delete fovSpinBox;
    delete fovLabel;
    delete cameraBoxLayout;
    delete cameraGroupBox;
    delete cameraOptionsLabel;

    delete layout;     
}

void RenderOptionsWidget::SwitchRender(int renderIndex) {
    if (renderIndex == 1) {
        rayTracingOptionsLabel->setVisible(true);
        rayTracingGroupBox->setVisible(true);
    }
    else {
        rayTracingOptionsLabel->setVisible(false);
        rayTracingGroupBox->setVisible(false);
    }
}

void RenderOptionsWidget::ShowAspectRatio(int state) {
    if (state == Qt::CheckState::Unchecked) {
        aspectRatioSpinBox->setVisible(true);
        aspectRatioSpinBox->setValue(camera->aspectRatio);
    }
    else {
        aspectRatioSpinBox->setVisible(false);
        emit ShouldResetAspectRatio();
    }
}

void RenderOptionsWidget::UpdateProperties() {
    camera->aspectRatio = aspectRatioSpinBox->value();
    camera->FOV = fovSpinBox->value();
    // called whenever an option in UI has been changed or should be updated
    emit ShouldUpdateGl();
}

void RenderOptionsWidget::PressedRayTrace() {
    emit ShouldRayTrace(samplesSpinBox->value()); // emit signal
}

void RenderOptionsWidget::PressedSaveImage() {
    bool ok;
    QString text = QInputDialog::getText(this, "Save Image", "Enter file name:", QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) 
        emit ShouldSaveImage(text);
}