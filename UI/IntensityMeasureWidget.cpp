#include "IntensityMeasureWidget.h"

IntensityMeasureWidget::IntensityMeasureWidget(IntensityMeasure &im, QWidget *parent): QWidget(parent), m_intensityMeasure(im)
{
    QGridLayout* layout = new QGridLayout();
    QGroupBox* imGroupBox = new QGroupBox(this);
    imGroupBox->setTitle("Intensity Measure");
    imGroupBox->setStyleSheet("QGroupBox {font-weight: bold;font-size: 10pt; qproperty-flat: true;}");

    QFormLayout* formLayout = new QFormLayout();
    m_typeBox = new QComboBox(this);

    formLayout->addRow(tr("Type:"), this->m_typeBox);
    imGroupBox->setLayout(formLayout);

    layout->addWidget(imGroupBox);
    this->setLayout(layout);

    const QStringList validType = this->m_intensityMeasure.validTypes();

    QStringListModel* typeModel = new QStringListModel(validType);
    m_typeBox->setModel(typeModel);
    this->setupConnections();
}

void IntensityMeasureWidget::setupConnections()
{
    connect(this->m_typeBox, &QComboBox::currentTextChanged,
            &this->m_intensityMeasure, &IntensityMeasure::setType);

    connect(&this->m_intensityMeasure, &IntensityMeasure::typeChanged,
            this->m_typeBox, &QComboBox::setCurrentText);
}
