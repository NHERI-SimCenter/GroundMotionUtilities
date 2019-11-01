#include "GMPEWidget.h"

GMPEWidget::GMPEWidget(GMPE& gmpe, QWidget *parent): QWidget(parent), m_gmpe(gmpe)
{
    QGridLayout* layout = new QGridLayout();
    QGroupBox* gmpeGroupBox = new QGroupBox(this);
    gmpeGroupBox->setTitle("Ground Motion Prediction Equation");

    QFormLayout* formLayout = new QFormLayout();
    m_typeBox = new QComboBox(this);

    formLayout->addRow(tr("Type:"), this->m_typeBox);
    gmpeGroupBox->setLayout(formLayout);

    layout->addWidget(gmpeGroupBox);
    this->setLayout(layout);

    const QStringList validType = this->m_gmpe.validTypes();

    QStringListModel* typeModel = new QStringListModel(validType);
    m_typeBox->setModel(typeModel);
    m_typeBox->setCurrentIndex(validType.indexOf(m_gmpe.type()));
    this->setupConnections();
}

void GMPEWidget::setupConnections()
{
    connect(this->m_typeBox, &QComboBox::currentTextChanged,
            &this->m_gmpe, &GMPE::setType);

    connect(&this->m_gmpe, &GMPE::typeChanged,
            this->m_typeBox, &QComboBox::setCurrentText);
}
