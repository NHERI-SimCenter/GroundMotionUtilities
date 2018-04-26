#include "GMWidget.h"
#include "SiteWidget.h"
#include <QtWidgets>
#include <QPalette>
#include <QtQuick/QQuickView>
#include <QQmlContext>

GMWidget::GMWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setAutoFillBackground(true);
    QPalette palette = QPalette();
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);
    this->m_site = new Site(37.8719, -122.2585);
    this->m_siteWidget = new SiteWidget(*this->m_site, this);
    QHBoxLayout* hBoxLayout = new QHBoxLayout(this);

    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(this->m_siteWidget, 0);

    RuptureLocation location(37.9,  -122.3);

    this->m_eqRupture = new PointSourceRupture(7.0, (const RuptureLocation&)location, 0.0, 90.0, this);
    this->m_ruptureWidget = new RuptureWidget(*this->m_eqRupture, this);
    vBoxLayout->addWidget(this->m_ruptureWidget, 0);

    this->m_gmpe = new GMPE();
    this->m_gmpeWidget = new GMPEWidget(*this->m_gmpe, this);
    vBoxLayout->addWidget(this->m_gmpeWidget);

    this->m_intensityMeasure = new IntensityMeasure();
    this->m_intensityMeasureWidget = new IntensityMeasureWidget(*this->m_intensityMeasure, this);
    vBoxLayout->addWidget(this->m_intensityMeasureWidget, 0);

    this->m_selectionconfig = new RecordSelectionConfig();
    this->m_selectionWidget = new RecordSelectionWidget(*this->m_selectionconfig, this);
    vBoxLayout->addWidget(this->m_selectionWidget, 0);

    m_scenarioProcessor = new ScenarioProcessor(*m_site, *m_eqRupture, *m_gmpe, *m_intensityMeasure, *m_selectionconfig);

    QPushButton* runButton = new QPushButton(tr("&Run"));
    connect(runButton, &QPushButton::released, this->m_scenarioProcessor, &ScenarioProcessor::processScenario);
    vBoxLayout->insertStretch(5, 1);

    vBoxLayout->addWidget(runButton);

    QQuickView *view = new QQuickView();
    view->rootContext()->setContextProperty("site", this->m_site);
    view->rootContext()->setContextProperty("siteLocation", &this->m_site->location());
    view->rootContext()->setContextProperty("rupture", this->m_eqRupture);

    view->setSource(QUrl("qrc:/ScenarioMap.qml"));
    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setMinimumSize(800, 600);

    hBoxLayout->addLayout(vBoxLayout);
    hBoxLayout->addWidget(container, 1);
    //this->setLayout(hBoxLayout);

    this->window()->setWindowTitle(tr("Simcenter - Earthquake Scenario Simulation"));
}

GMWidget::~GMWidget()
{
}
