#include "GmAppConfigWidget.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QFileDialog>

GmAppConfigWidget::GmAppConfigWidget(GmAppConfig &appConfig, QWidget *parent) : QWidget(parent, Qt::Dialog), m_appConfig(appConfig)
{
    this->setWindowModality(Qt::WindowModal);
    this->setWindowTitle("Path Settings");
    this->setWindowIcon(QIcon(":/images/settings.png"));
    this->setAutoFillBackground(true);
    QPalette palette = QPalette();
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);

    QFormLayout* formLayout = new QFormLayout(this);

    QHBoxLayout* eqHazardBoxLayout = new QHBoxLayout();
    m_eqHazardBox = new QLineEdit(this);
    eqHazardBoxLayout->addWidget(m_eqHazardBox, 1);
    m_eqHazardButton = new QPushButton(tr("Browse..."), this);
    eqHazardBoxLayout->addWidget(m_eqHazardButton, 0);
    formLayout->addRow("EQ Hazard App:", eqHazardBoxLayout);

    QHBoxLayout* simulateIMBoxLayout = new QHBoxLayout();
    m_simulateIMBox = new QLineEdit(this);
    simulateIMBoxLayout->addWidget(m_simulateIMBox, 1);
    m_simulateIMButton = new QPushButton(tr("Browse..."), this);
    simulateIMBoxLayout->addWidget(m_simulateIMButton, 0);
    formLayout->addRow("IM Simulation App:", simulateIMBoxLayout);

    QHBoxLayout* selectRecordBoxLayout = new QHBoxLayout();
    m_selectRecordBox = new QLineEdit(this);
    selectRecordBoxLayout->addWidget(m_selectRecordBox, 1);
    m_selectRecordButton = new QPushButton(tr("Browse..."), this);
    selectRecordBoxLayout->addWidget(m_selectRecordButton, 0);
    formLayout->addRow("Record Selection App:", selectRecordBoxLayout);

    QHBoxLayout* ngaw2DbBoxLayout = new QHBoxLayout();
    m_ngaw2DbBox = new QLineEdit(this);
    ngaw2DbBoxLayout->addWidget(m_ngaw2DbBox, 1);
    m_ngaw2DbButton = new QPushButton(tr("Browse..."), this);
    ngaw2DbBoxLayout->addWidget(m_ngaw2DbButton, 0);
    formLayout->addRow("NGA West 2 Database:", ngaw2DbBoxLayout);

    QHBoxLayout* ngaw2DbSubsetBoxLayout = new QHBoxLayout();
    m_ngaw2DbSubsetBox = new QLineEdit(this);
    ngaw2DbSubsetBoxLayout->addWidget(m_ngaw2DbSubsetBox, 1);
    m_ngaw2DbSubsetButton = new QPushButton(tr("Browse..."), this);
    ngaw2DbSubsetBoxLayout->addWidget(m_ngaw2DbSubsetButton, 0);
    formLayout->addRow("NGA West 2 Subset:", ngaw2DbSubsetBoxLayout);

    //Reading current path
    m_eqHazardBox->setText(m_appConfig.eqHazardPath());
    m_simulateIMBox->setText(m_appConfig.simulateIMPath());
    m_selectRecordBox->setText(m_appConfig.selectRecordPath());
    m_ngaw2DbBox->setText(m_appConfig.NGAW2DbPath());
    m_ngaw2DbSubsetBox->setText(m_appConfig.NGAW2SubsetDbPath());

    this->setMinimumWidth(600);
    setupConnections();
}

void GmAppConfigWidget::setupConnections()
{
    connect(m_eqHazardButton, &QPushButton::released, [this](){
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter("JAR files (*.jar)");
        if(dialog.exec())
            m_appConfig.setEqHazardPath(dialog.selectedFiles()[0]);
    });

    connect(&m_appConfig, &GmAppConfig::eqHazardPathChanged, m_eqHazardBox, &QLineEdit::setText);

    connect(m_simulateIMButton, &QPushButton::released, [this](){
        QFileDialog dialog(this);
        dialog.setFilter(QDir::Executable);

        if(dialog.exec())
            m_appConfig.setSimulateIMPath(dialog.selectedFiles()[0]);
    });

    connect(&m_appConfig, &GmAppConfig::simulateIMPathChanged, m_simulateIMBox, &QLineEdit::setText);

    connect(m_selectRecordButton, &QPushButton::released, [this](){
        QFileDialog dialog(this);
        dialog.setFilter(QDir::Executable);

        if(dialog.exec())
            m_appConfig.setSelectRecordPath(dialog.selectedFiles()[0]);
    });

    connect(&m_appConfig, &GmAppConfig::selectRecordPathChanged, m_selectRecordBox, &QLineEdit::setText);

    connect(m_ngaw2DbButton, &QPushButton::released, [this](){
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter("Comma Separated Values (*.csv)");

        if(dialog.exec())
            m_appConfig.setNGAW2DbPath(dialog.selectedFiles()[0]);
    });

    connect(&m_appConfig, &GmAppConfig::NGAW2DbPathChanged, m_ngaw2DbBox, &QLineEdit::setText);

    connect(m_ngaw2DbSubsetButton, &QPushButton::released, [this](){
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter("Comma Separated Values (*.csv)");

        if(dialog.exec())
            m_appConfig.setNGAW2SubsetDbPath(dialog.selectedFiles()[0]);
    });

    connect(&m_appConfig, &GmAppConfig::NGAW2SubsetDbChanged, m_ngaw2DbSubsetBox, &QLineEdit::setText);
}
