#include "RecordSelectionWidget.h"

RecordSelectionWidget::RecordSelectionWidget(RecordSelectionConfig& selectionConfig, QWidget *parent) : QWidget(parent), m_selectionConfig(selectionConfig)
{
    QGridLayout* layout = new QGridLayout();
    QGroupBox* selectionGroupBox = new QGroupBox(this);
    selectionGroupBox->setTitle("Record Selection");
    selectionGroupBox->setStyleSheet("QGroupBox {font-weight: bold;font-size: 10pt; qproperty-flat: true;}");

    QFormLayout* formLayout = new QFormLayout();
    m_dbBox = new QComboBox(this);
    formLayout->addRow(tr("Database:"), this->m_dbBox);
    m_dbBox->addItem("NGA West 2", RecordSelectionConfig::RecordDb::NGAWest2);

    m_dbBox->setCurrentIndex(m_dbBox->findData(this->m_selectionConfig.database()));

    m_errorBox = new QComboBox(this);
    formLayout->addRow(tr("Error Metric:"), this->m_errorBox);
    m_errorBox->addItem("Absolute Sum", RecordSelectionConfig::ErrorMetric::AbsSum);
    m_errorBox->addItem("Root Mean Square", RecordSelectionConfig::ErrorMetric::RMSE);
    m_errorBox->addItem("Mean Square", RecordSelectionConfig::ErrorMetric::MSE);
    m_errorBox->addItem("Mean Absolute Percent Error", RecordSelectionConfig::ErrorMetric::MAPE);
    m_errorBox->setCurrentIndex(m_errorBox->findData(this->m_selectionConfig.error()));

    selectionGroupBox->setLayout(formLayout);

    layout->addWidget(selectionGroupBox);
    this->setLayout(layout);

    //Now, we will connect the ui to the data
    this->setupConnections();
}

void RecordSelectionWidget::setupConnections()
{
    //Connecting database
    connect(this->m_dbBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](const int& index)
            {
                RecordSelectionConfig::RecordDb dbType = qvariant_cast<RecordSelectionConfig::RecordDb>(m_dbBox->itemData(index));
                m_selectionConfig.setDatabase(dbType);
            });

    connect(&this->m_selectionConfig, &RecordSelectionConfig::databaseChanged,
            [this](RecordSelectionConfig::RecordDb database)
            {
                this->m_dbBox->setCurrentIndex(this->m_dbBox->findData(database));
            });

    //Connecting error
    connect(this->m_errorBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](const int& index)
            {
                RecordSelectionConfig::ErrorMetric errorType = qvariant_cast<RecordSelectionConfig::ErrorMetric>(m_errorBox->itemData(index));
                m_selectionConfig.setError(errorType);
            });

    connect(&this->m_selectionConfig, &RecordSelectionConfig::errorChanged,
            [this](RecordSelectionConfig::ErrorMetric error)
            {
                this->m_errorBox->setCurrentIndex(this->m_errorBox->findData(error));
            });
}
