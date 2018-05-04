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

    Qt::Orientation orientation = Qt::Horizontal;

    this->m_siteWidget = new SiteWidget(*this->m_site, this, orientation);
    QHBoxLayout* hBoxLayout = new QHBoxLayout(this);

    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(this->m_siteWidget, 0);

    RuptureLocation location(37.9,  -122.3);

    this->m_eqRupture = new PointSourceRupture(7.0, (const RuptureLocation&)location, 0.0, 90.0, this);
    this->m_ruptureWidget = new RuptureWidget(*this->m_eqRupture, this, orientation);
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

    this->m_siteResult = new SiteResult();
    m_scenarioProcessor = new ScenarioProcessor(*m_site, *m_eqRupture, *m_gmpe, *m_intensityMeasure, *m_selectionconfig, *m_siteResult);

    QPushButton* runButton = new QPushButton(tr("&Run"));
    runButton->setMinimumSize(64, 32);
    connect(runButton, &QPushButton::released, this->m_scenarioProcessor, &ScenarioProcessor::startProcessingScenario);

    QPushButton* settingButton = new QPushButton(QIcon(":/images/settings.png"), tr("&Settings"));
    settingButton->setMinimumSize(96, 32);
    settingButton->setIconSize(QSize(25,25));

    QWidget* bottomToolsWidget = new QWidget();
    QHBoxLayout* bottomToolsHBox = new QHBoxLayout();
    bottomToolsWidget->setLayout(bottomToolsHBox);
    bottomToolsHBox->addWidget(settingButton);
    bottomToolsHBox->addStretch();
    bottomToolsHBox->addWidget(runButton);

    vBoxLayout->insertStretch(5, 1);
    vBoxLayout->addWidget(bottomToolsWidget);


    QQuickView *view = new QQuickView();
    view->rootContext()->setContextProperty("site", this->m_site);
    view->rootContext()->setContextProperty("siteLocation", &this->m_site->location());
    view->rootContext()->setContextProperty("rupture", this->m_eqRupture);
    view->rootContext()->setContextProperty("siteResult", this->m_siteResult);

    view->setSource(QUrl("qrc:/ScenarioMap.qml"));
    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setMinimumSize(400, 400);
    container->resize(600, container->height());

    QSplitter* splitter = new QSplitter(Qt::Orientation::Horizontal, this);
    //hBoxLayout->addLayout(vBoxLayout);
    QWidget* toolsWidget = new QWidget(this);
    toolsWidget->setLayout(vBoxLayout);
    splitter->addWidget(toolsWidget);
    splitter->addWidget(container);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    hBoxLayout->addWidget(splitter, 1);
    this->resize(1000, this->height());
    this->window()->setWindowTitle(tr("Simcenter - Earthquake Scenario Simulation"));
}

GMWidget::~GMWidget()
{
}
