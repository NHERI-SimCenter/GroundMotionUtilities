#include "GMWidget.h"
#include "SiteWidget.h"
#include <QtWidgets>
#include <QPalette>
#include <QtQuick/QQuickView>
#include <QQmlContext>

GMWidget::GMWidget(QWidget *parent) :
    QWidget(parent)
{
    m_mode = ApplicationMode::Edit;
    m_locationsModel = new LocationsListModel(this);

    this->setAutoFillBackground(true);
    Qt::Orientation orientation = Qt::Horizontal;

    QPalette palette = QPalette();
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);

    QHBoxLayout* hBoxLayout = new QHBoxLayout(this);

    QVBoxLayout* vBoxLayout = new QVBoxLayout();

    //Adding Site Config Widget
    m_siteConfig = new SiteConfig();
    m_siteConfigWidget = new SiteConfigWidget(*m_siteConfig);
    vBoxLayout->addWidget(this->m_siteConfigWidget, 0);

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
    m_scenarioProcessor = new ScenarioProcessor(*m_siteConfig, *m_eqRupture, *m_gmpe, *m_intensityMeasure, *m_selectionconfig, *m_siteResult);

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
    qmlRegisterType<SiteConfig>("org.designsafe.ci.simcenter", 1, 0, "SiteConfig");
    qmlRegisterType<GMWidget>("org.designsafe.ci.simcenter", 1, 0, "GMWidget");

    view->rootContext()->setContextProperty("site", &m_siteConfig->site());
    view->rootContext()->setContextProperty("rupture", this->m_eqRupture);
    view->rootContext()->setContextProperty("siteResult", this->m_siteResult);
    view->rootContext()->setContextProperty("siteGrid", &m_siteConfig->siteGrid());
    view->rootContext()->setContextProperty("siteConfig", m_siteConfig);
    view->rootContext()->setContextProperty("siteResultsModel", &m_scenarioProcessor->getResultsModel());
    view->rootContext()->setContextProperty("gmApp", this);
    view->rootContext()->setContextProperty("locationsModel", m_locationsModel);

    view->setSource(QUrl("qrc:/ScenarioMap.qml"));
    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setMinimumSize(400, 400);
    container->resize(600, container->height());
    m_mapObject = view->rootObject();


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

    updateLocations();
    setupConnections();
}

GMWidget::~GMWidget()
{
}

GMWidget::ApplicationMode GMWidget::mode() const
{
    return m_mode;
}

void GMWidget::setMode(GMWidget::ApplicationMode mode)
{
    if(m_mode != mode)
    {
        m_mode = mode;
        emit modeChanged(m_mode);
    }
}

void GMWidget::updateLocations()
{
    QObject* gridMarker = m_mapObject->findChild<QObject*>("siteGridMarker");
    QVariant dragged = gridMarker->property("dragged");
    bool isDragged = dragged.toBool();

    if(!isDragged)
    {
        QVector<Location*> locations;
        double dTol =  1e-3;
        GridDivision& latitude = m_siteConfig->siteGrid().latitude();
        GridDivision& longitude = m_siteConfig->siteGrid().longitude();
        for (double alat = latitude.min(); alat <= latitude.max() + dTol; alat += latitude.getStep())
        {
            for (double alon = longitude.min(); alon <= longitude.max() + dTol; alon += longitude.getStep())
            {
                locations.append(new Location(alat, alon));
            }
        }
        m_locationsModel->setLocations(locations);
    }
}

void GMWidget::setupConnections()
{
    connect(m_scenarioProcessor, &ScenarioProcessor::finished, [this]()
    {
        setMode(GMWidget::ApplicationMode::Results);
    });

    //Connecting changes in input to edit mode
    //Connecting grid latitude
    connect(&m_siteConfig->siteGrid().latitude(), &GridDivision::minChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(&m_siteConfig->siteGrid().latitude(), &GridDivision::maxChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(&m_siteConfig->siteGrid().latitude(), &GridDivision::divisionsChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    //Connecting grid longitude
    connect(&m_siteConfig->siteGrid().longitude(), &GridDivision::minChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(&m_siteConfig->siteGrid().longitude(), &GridDivision::maxChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(&m_siteConfig->siteGrid().longitude(), &GridDivision::divisionsChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(m_siteConfig, &SiteConfig::typeChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(m_eqRupture, &PointSourceRupture::magnitudeChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(m_eqRupture, &PointSourceRupture::dipChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(m_eqRupture, &PointSourceRupture::rakeChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(&m_eqRupture->location(), &RuptureLocation::latitudeChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(&m_eqRupture->location(), &RuptureLocation::longitudeChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(&m_eqRupture->location(), &RuptureLocation::depthChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    //Connecting changes in the site grid to update in locations list
    connect(&m_siteConfig->siteGrid().latitude(), &GridDivision::minChanged, [this](){updateLocations();});
    connect(&m_siteConfig->siteGrid().latitude(), &GridDivision::maxChanged, [this](){updateLocations();});
    connect(&m_siteConfig->siteGrid().latitude(), &GridDivision::divisionsChanged, [this](){updateLocations();});
    connect(&m_siteConfig->siteGrid().longitude(), &GridDivision::minChanged, [this](){updateLocations();});
    connect(&m_siteConfig->siteGrid().longitude(), &GridDivision::maxChanged, [this](){updateLocations();});
    connect(&m_siteConfig->siteGrid().longitude(), &GridDivision::divisionsChanged, [this](){updateLocations();});

}
