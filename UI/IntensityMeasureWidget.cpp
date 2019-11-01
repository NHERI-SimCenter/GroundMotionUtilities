#include "IntensityMeasureWidget.h"

IntensityMeasureWidget::IntensityMeasureWidget(IntensityMeasure &im, QWidget *parent): QWidget(parent), m_intensityMeasure(im)
{
    QGridLayout* layout = new QGridLayout();
    QGroupBox* imGroupBox = new QGroupBox(this);
    imGroupBox->setTitle("Intensity Measure");

    QFormLayout* formLayout = new QFormLayout();
    m_typeBox = new QComboBox(this);

    formLayout->addRow(tr("Type:"), this->m_typeBox);
    imGroupBox->setLayout(formLayout);

    m_isCorrelatedBox = new QCheckBox();
    formLayout->addRow(tr("Spatial Correlation"), m_isCorrelatedBox);
    m_isCorrelatedBox->setChecked(m_intensityMeasure.IsCorrelated());

    layout->addWidget(imGroupBox);
    this->setLayout(layout);

    const QStringList validType = this->m_intensityMeasure.validTypes();

    m_typeBox->addItem("Peak Ground Acceleration (PGA)", "PGA");
    m_typeBox->addItem("Spectral Accelerations (SA)", "SA");
    m_typeBox->addItem("All Supported IMs", "All");

    m_typeBox->setCurrentIndex(validType.indexOf(m_intensityMeasure.type()));

    this->setupConnections();
}

void IntensityMeasureWidget::setupConnections()
{
    connect(this->m_typeBox, &QComboBox::currentTextChanged,
            &this->m_intensityMeasure, &IntensityMeasure::setType);

    connect(&this->m_intensityMeasure, &IntensityMeasure::typeChanged,
            this->m_typeBox, &QComboBox::setCurrentText);

    connect(&this->m_intensityMeasure, &IntensityMeasure::correlationChanged,
            this->m_isCorrelatedBox, &QCheckBox::setChecked);

    connect(this->m_isCorrelatedBox, &QCheckBox::stateChanged,  [this](){
        if(m_isCorrelatedBox->isChecked())
            m_intensityMeasure.setIsCorrelated(true);
        else
            m_intensityMeasure.setIsCorrelated(false);
    });
}
