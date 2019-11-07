#include "GMWidget.h"
#include "SiteWidget.h"
#include <QtWidgets>
#include <QPalette>
#include <QtQuick/QQuickView>
#include <QQmlContext>
#include "../../SimCenterCommon/Common/FooterWidget.h"
#include "GmAppConfigWidget.h"
#include "GmCommon.h"

GMWidget::GMWidget(QWidget *parent) :
    QWidget(parent)
{
    initAppConfig();
    initActions();

    m_mode = ApplicationMode::Edit;
    m_locationsModel = new LocationsListModel(this);

    this->setAutoFillBackground(true);
    this->setWindowIcon(QIcon(":/images/earthquake.svg"));

    Qt::Orientation orientation = Qt::Horizontal;

    QPalette palette = QPalette();
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);

    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);

    QVBoxLayout* toolsVBoxLayout = new QVBoxLayout();

    //Adding Site Config Widget
    m_siteConfig = new SiteConfig();
    m_siteConfigWidget = new SiteConfigWidget(*m_siteConfig);
    toolsVBoxLayout->addWidget(this->m_siteConfigWidget, 0);

    RuptureLocation location(37.9,  -122.3);

    this->m_eqRupture = new PointSourceRupture(7.0, (const RuptureLocation&)location, 0.0, 90.0, this);
    this->m_ruptureWidget = new RuptureWidget(*this->m_eqRupture, this, orientation);
    toolsVBoxLayout->addWidget(this->m_ruptureWidget, 0);

    this->m_gmpe = new GMPE();
    this->m_gmpeWidget = new GMPEWidget(*this->m_gmpe, this);
    toolsVBoxLayout->addWidget(this->m_gmpeWidget);

    this->m_intensityMeasure = new IntensityMeasure();
    this->m_intensityMeasureWidget = new IntensityMeasureWidget(*this->m_intensityMeasure, this);
    toolsVBoxLayout->addWidget(this->m_intensityMeasureWidget, 0);

    this->m_selectionconfig = new RecordSelectionConfig();
    this->m_selectionWidget = new RecordSelectionWidget(*this->m_selectionconfig, this);
    toolsVBoxLayout->addWidget(this->m_selectionWidget, 0);

    this->m_siteResult = new SiteResult();
    m_scenarioProcessor = new ScenarioProcessor(*m_appConfig, *m_siteConfig, *m_eqRupture, *m_gmpe,
                                                *m_intensityMeasure, *m_selectionconfig, *m_siteResult);

    m_runButton = new QPushButton(tr("&Run"));
    m_runButton->setMinimumSize(64, 32);

    m_settingButton = new QPushButton(QIcon(":/images/settings.png"), tr("&Settings"));
    m_settingButton->setMinimumSize(96, 32);
    m_settingButton->setIconSize(QSize(25,25));

    QWidget* bottomToolsWidget = new QWidget();
    QHBoxLayout* bottomToolsHBox = new QHBoxLayout();
    bottomToolsWidget->setLayout(bottomToolsHBox);
    bottomToolsHBox->addWidget(m_settingButton);

    //Adding alert icon
    QIcon alertIcon(":/images/Alert.svg");
    m_alertIconWidget = new QLabel();
    m_alertIconWidget->setPixmap(alertIcon.pixmap(30, 30));
    m_alertIconWidget->setToolTip("The application is not configured properly, please go to setting to set the paths!");
    bottomToolsHBox->addWidget(m_alertIconWidget);
    m_alertIconWidget->setHidden(true);

    bottomToolsHBox->addStretch();
    bottomToolsHBox->addWidget(m_runButton);

    toolsVBoxLayout->insertStretch(5, 1);
    toolsVBoxLayout->addWidget(bottomToolsWidget);

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

    view->setSource(QUrl("qrc:/ScenarioMapContainer.qml"));
    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setMinimumSize(400, 400);
    container->resize(600, container->height());
    m_mapObject = view->rootObject();

    QSplitter* splitter = new QSplitter(Qt::Orientation::Horizontal, this);
    //hBoxLayout->addLayout(vBoxLayout);
    QWidget* toolsWidget = new QWidget(this);
    toolsWidget->setLayout(toolsVBoxLayout);
    splitter->addWidget(toolsWidget);
    splitter->addWidget(container);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    //Adding site results widget
    m_resultsWidget = new SiteResultsWidget();
    m_resultsWidget->hide();
    splitter->addWidget(m_resultsWidget);
    splitter->setStretchFactor(2, 0);
    view->rootContext()->setContextProperty("resultsWidget", m_resultsWidget);

    vBoxLayout->addWidget(splitter, 1);
    this->resize(1000, this->height());
    this->window()->setWindowTitle(tr("Simcenter - Earthquake Scenario Simulation"));

    //Adding Simcenter footer
    FooterWidget* footer = new FooterWidget(this);
    vBoxLayout->addWidget(footer);

    //Adding a status bar
    m_statusBar = new QStatusBar(this);
    m_mainStatus = new QLabel("");
    m_mainStatus->setMinimumWidth(200);
    m_mainStatus->setMaximumWidth(400);
    m_statusBar->addWidget(m_mainStatus, 1);

    m_detailedStatus = new QLabel("");
    m_detailedStatus->setMinimumWidth(500);
    m_detailedStatus->setMaximumWidth(800);
    m_statusBar->addWidget(m_detailedStatus, 2);

    QWidget* spacer = new QWidget();
    m_statusBar->addWidget(spacer, 1);

    m_modeStatus = new QLabel("Mode: Edit");
    m_modeStatus->setMinimumWidth(120);
    m_modeStatus->setMaximumWidth(150);
    m_statusBar->addWidget(m_modeStatus, 0.2);

    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 100);
    m_progressBar->setVisible(false);
    m_progressBar->setMinimumWidth(200);
    QSizePolicy progressSizePolicy = m_progressBar->sizePolicy();
    progressSizePolicy.setRetainSizeWhenHidden(true);
    m_progressBar->setSizePolicy(progressSizePolicy);
    m_progressBar->setMaximumWidth(200);
    m_statusBar->addWidget(m_progressBar, 0);
    m_statusBar->setStyleSheet("QStatusBar::item { border: 1px solid #F0F8FF; border-radius: 3px; }");

    QMenuBar* menuBar = new QMenuBar();
    QMenu *fileMenu = new QMenu(tr("&File"));
    menuBar->addMenu(fileMenu);
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_loadAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exportAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exitAction);

    QMenu *editMenu = new QMenu(tr("&Edit"));
    QMenu* presetsMenu = editMenu->addMenu(tr("&Presets"));
    presetsMenu->addAction(m_setLAAction);
    presetsMenu->addAction(m_setSFAction);
    presetsMenu->addAction(m_setSEAAction);
    presetsMenu->addAction(m_setANCAction);
    editMenu->addSeparator();
    editMenu->addAction(m_settingsAction);
    menuBar->addMenu(editMenu);

    QMenu *viewMenu = new QMenu(tr("&View"));
    QMenu *mapsMenu = viewMenu->addMenu(tr("&Map"));
    mapsMenu->addAction(m_defaultMapAction);
    mapsMenu->addAction(m_terrainMapAction);
    menuBar->addMenu(viewMenu);

    QMenu *helpMenu = new QMenu(tr("&Help"));
    helpMenu->addAction(m_aboutAction);
    helpMenu->addAction(m_licenseAction);
    menuBar->addMenu(helpMenu);

    this->layout()->setMenuBar(menuBar);
    vBoxLayout->addWidget(m_statusBar);
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

void GMWidget::setAppConfig()
{
    GmAppConfigWidget* configWidget = new GmAppConfigWidget(*m_appConfig, this);
    configWidget->show();
}

void GMWidget::exportAll()
{
    QString exportDir = QFileDialog::getExistingDirectory(this);
    if(!exportDir.isEmpty())
    {
        exportFile("Scenario.json", exportDir);
        exportFile("Scenario_SHA.json", exportDir);
        exportFile("Scenario_SHA_Geo.json", exportDir);
        exportFile("SimConfig.json", exportDir);
        exportFile("SimOutput.json", exportDir);
        exportFile("SelectionConfig.json", exportDir);
        exportFile("SelectionOutput.json", exportDir);

    }
}

void GMWidget::setupConnections()
{    
    //Connecting the run button
    connect(m_runButton, &QPushButton::released, [this]()
    {
        if(m_appConfig->validate())
        {
            this->m_scenarioProcessor->startProcessingScenario();
            //
            NGARecordsDb& db = NGARecordsDb::getInstance(m_appConfig->NGAW2DbPath());
            m_resultsWidget->setRecordsDb(&db);
            m_progressBar->setVisible(true);
            m_alertIconWidget->setHidden(true);
            saveAppSettings();
        }
        else
        {
            m_alertIconWidget->setHidden(false);
            m_progressBar->setVisible(false);
            m_mainStatus->setText("Invalid configuration!");
        }
    });

    connect(m_settingButton, &QPushButton::released, this, &GMWidget::setAppConfig);

    //connecting scenario to status bar
    connect(m_scenarioProcessor, &ScenarioProcessor::finished, [this]()
    {
        setMode(GMWidget::ApplicationMode::Results);
        m_progressBar->setVisible(false);
    });

    connect(this, &GMWidget::modeChanged, [this]()
    {
        if(m_mode == ApplicationMode::Edit)
        {
            m_modeStatus->setText("Mode: Edit");
            m_exportAction->setDisabled(true);
        }
        else
        {
            m_modeStatus->setText("Mode: Results");
            m_exportAction->setDisabled(false);
        }
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

    //Connecting input changes to mode
    connect(&m_siteConfig->siteGrid().longitude(), &GridDivision::divisionsChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    connect(m_siteConfig, &SiteConfig::typeChanged, [this]()
    {
        setMode(GMWidget::ApplicationMode::Edit);
    });

    //Connecting rupture
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

    //Connecting Scenario processor to status bar
    connect(m_scenarioProcessor, &ScenarioProcessor::statusUpdated, [this](QString status){
       m_mainStatus->setText(status);
    });

    connect(m_scenarioProcessor, QOverload<QString>::of(&ScenarioProcessor::progressUpdated), [this](QString progress){
       m_detailedStatus->setText(progress);
    });

    connect(m_scenarioProcessor, QOverload<double>::of(&ScenarioProcessor::progressUpdated), [this](double progress){
       m_progressBar->setValue(static_cast<int>(progress*100));
    });

    //Connecting Actions
    connect(m_exitAction, &QAction::triggered, QApplication::instance(), &QApplication::quit);
    connect(m_settingsAction, &QAction::triggered, this, &GMWidget::setAppConfig);
    connect(m_setLAAction, &QAction::triggered, [this]()
    {
        m_siteConfig->siteGrid().setCenter(33.955, -117.9195);
        m_siteConfig->siteGrid().latitude().set(33.713, 34.197, 25);
        m_siteConfig->siteGrid().longitude().set(-118.527, -117.312, 40);
        m_siteConfig->site().setLocation(34.0522, -118.2437);
        m_eqRupture->location().set(33.892, -117.779);
        setMapCenter(33.955, -117.9195);
    });

    connect(m_setSFAction, &QAction::triggered, [this]()
    {
        m_siteConfig->siteGrid().setCenter(37.7, -122.1245);
        m_siteConfig->siteGrid().latitude().set(37.301, 38.100, 30);
        m_siteConfig->siteGrid().longitude().set(-122.519, -121.730, 30);
        m_siteConfig->site().setLocation(37.8719, -122.2585);
        m_eqRupture->location().set(37.770, -122.128);
        setMapCenter(37.7, -122.1245);
    });

    connect(m_setSEAAction, &QAction::triggered, [this]()
    {
        m_siteConfig->siteGrid().setCenter(47.6325, -122.133);
        m_siteConfig->siteGrid().latitude().set(47.258, 48.007, 40);
        m_siteConfig->siteGrid().longitude().set(-122.395, -121.871, 25);

        m_siteConfig->site().setLocation(47.6062, -122.3321);
        m_eqRupture->location().set(47.591, -122.158);
        setMapCenter(47.6325, -122.133);
    });

    connect(m_setANCAction, &QAction::triggered, [this]()
    {
        m_siteConfig->siteGrid().setCenter(61.1658, -149.854);
        m_siteConfig->siteGrid().latitude().set(60.936, 61.337, 40);
        m_siteConfig->siteGrid().longitude().set(-150.073, -149.045, 25);

        m_siteConfig->site().setLocation(61.1658, -149.854);
        m_eqRupture->location().set(61.346, -149.955);
        m_eqRupture->setMagnitude(7.0);
        m_eqRupture->location().setDepth(46.7);
        m_siteConfig->siteGrid().latitude().setDivisions(40);
        m_siteConfig->siteGrid().longitude().setDivisions(40);
        m_intensityMeasure->setIsCorrelated(false);
        m_gmpe->setType("Zhao Et Al. (2006) - Intraslab");
        m_selectionconfig->setDatabase(RecordSelectionConfig::RecordDb::NGAWest2);
        setMapCenter(61.1658, -149.854);
    });


    connect(m_exportAction, &QAction::triggered, this, &GMWidget::exportAll);

    connect(m_terrainMapAction, &QAction::triggered, this, [this](){
        QMetaObject::invokeMethod(m_mapObject, "setTerrainMap");
    });

    connect(m_defaultMapAction, &QAction::triggered, this, [this](){
        QMetaObject::invokeMethod(m_mapObject, "setDefaultMap");
    });
}

void GMWidget::initAppConfig()
{
    m_appConfig = new GmAppConfig(this);

    //First, We will look into settings
    QSettings settings;
    m_appConfig->setEqHazardPath(settings.value("EqHazardPath", "").toString());
    m_appConfig->setSimulateIMPath(settings.value("SimulateIMPath", "").toString());
    m_appConfig->setSelectRecordPath(settings.value("SelectRecordPath", "").toString());
    m_appConfig->setNGAW2DbPath(settings.value("NGAW2DbPath", "").toString());
    m_appConfig->setNGAW2SubsetDbPath(settings.value("NGAW2SubsetDbPath", "").toString());

    //If path is missing we will try relative path
    //TODO: we need to search for the applications and files automatically (maybe platform dependent)
    if(m_appConfig->eqHazardPath().isEmpty() || !QFile::exists(m_appConfig->eqHazardPath()))
    {
        if(QFileInfo::exists("../EQHazard/build/EQHazard.jar"))
            m_appConfig->setEqHazardPath(QFileInfo("../EQHazard/build/EQHazard.jar").absoluteFilePath());
    }

    if(m_appConfig->simulateIMPath().isEmpty() || !QFile::exists(m_appConfig->simulateIMPath()))
    {
        if(QFileInfo::exists("./SimulateIM/Release/SimulateIM.exe"))
            m_appConfig->setSimulateIMPath(QFileInfo("./SimulateIM/Release/SimulateIM.exe").absoluteFilePath());
    }

    if(m_appConfig->selectRecordPath().isEmpty() || !QFile::exists(m_appConfig->selectRecordPath()))
    {
        if(QFileInfo::exists("./SelectRecord/Release/SelectRecord.exe"))
            m_appConfig->setSelectRecordPath(QFileInfo("./SelectRecord/Release/SelectRecord.exe").absoluteFilePath());
    }

    if(m_appConfig->NGAW2DbPath().isEmpty() || !QFile::exists(m_appConfig->NGAW2DbPath()))
    {
        if(QFileInfo::exists("./SelectRecord/Release/NGAWest2.csv"))
            m_appConfig->setNGAW2DbPath(QFileInfo("./SelectRecord/Release/NGAWest2.csv").absoluteFilePath());
    }

    if(m_appConfig->NGAW2SubsetDbPath().isEmpty() || !QFile::exists(m_appConfig->NGAW2SubsetDbPath()))
    {
        if(QFileInfo::exists("./SelectRecord/Release/NGAWest2-1000.csv"))
            m_appConfig->setNGAW2SubsetDbPath(QFileInfo("./SelectRecord/Release/NGAWest2-1000.csv").absoluteFilePath());
    }
}

void GMWidget::initActions()
{
    m_saveAction = new QAction(tr("&Save Scenario"), this);
    m_saveAction->setShortcut(QKeySequence::Save);

    m_loadAction = new QAction(tr("&Load Scenario"), this);
    m_loadAction->setShortcut(QKeySequence::Open);

    m_exportAction = new QAction(tr("Ex&port all"), this);
    m_exportAction->setShortcut(QKeySequence::SaveAs);
    m_exportAction->setDisabled(true);

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcut(QKeySequence::Close);

    m_setLAAction = new QAction(tr("Los &Angeles"), this);
    m_setSFAction = new QAction(tr("San &Francisco"), this);
    m_setSEAAction = new QAction(tr("Sea&ttle"), this);
    m_setANCAction = new QAction(tr("An&chorage"), this);

    m_settingsAction = new QAction(tr("Settin&gs"), this);

    m_aboutAction = new QAction(tr("A&bout"), this);
    m_licenseAction = new QAction(tr("&License"), this);

    m_defaultMapAction = new QAction(tr("&Default"));
    m_terrainMapAction = new QAction(tr("Te&rrain"));
}

void GMWidget::saveAppSettings()
{
    QSettings settings;
    settings.setValue("EqHazardPath", m_appConfig->eqHazardPath());
    settings.setValue("SimulateIMPath", m_appConfig->simulateIMPath());
    settings.setValue("SelectRecordPath", m_appConfig->selectRecordPath());
    settings.setValue("NGAW2DbPath", m_appConfig->NGAW2DbPath());
    settings.setValue("NGAW2SubsetDbPath", m_appConfig->NGAW2SubsetDbPath());
}

void GMWidget::setMapCenter(double latitude, double longitude)
{
    QObject* map = m_mapObject->findChild<QObject*>("map");
    if (map)
    {
        m_mapCenter.setLatitude(latitude);
        m_mapCenter.setLongitude(longitude);
        map->setProperty("center", QVariant::fromValue(m_mapCenter));
        map->setProperty("zoomLevel", 9.75);
    }
}

void GMWidget::exportFile(QString fileName, QString directory)
{
    QString workFilePath = GmCommon::getWorkFilePath(fileName);
    if(QFileInfo::exists(workFilePath))
        QFile::copy(workFilePath, QDir::cleanPath(directory + QDir::separator() + fileName));
}

QString GMWidget::getCacheLocation()
{
    return GmCommon::getCacheLocation();
}
