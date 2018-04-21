#include "GMWidget.h"
#include "ui_GMWidget.h"
#include "SiteWidget.h"
#include <QtWidgets/QHBoxLayout>

GMWidget::GMWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GMWidget)
{
    //ui->setupUi(this);

    this->m_site = new Site(37.9, -122.3);
    this->m_siteWidget = new SiteWidget(*this->m_site, this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(this->m_siteWidget);

    RuptureLocation location(0.0, 0.0);

    this->m_eqRupture = new PointSourceRupture(7.0, (const RuptureLocation&)location, 0.0, 90.0, this);
    this->m_ruptureWidget = new RuptureWidget(*this->m_eqRupture, this);
    layout->addWidget(this->m_ruptureWidget);

    this->m_gmpe = new GMPE();
    this->m_gmpeWidget = new GMPEWidget(*this->m_gmpe, this);
    layout->addWidget(this->m_gmpeWidget);

    this->m_intensityMeasure = new IntensityMeasure();
    this->m_intensityMeasureWidget = new IntensityMeasureWidget(*this->m_intensityMeasure, this);
    layout->addWidget(this->m_intensityMeasureWidget);

    this->setLayout(layout);

    this->window()->setWindowTitle("Simcenter - Earthquake Scenario Simulation");
}

GMWidget::~GMWidget()
{
    delete ui;
}
