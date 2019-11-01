#ifndef GMWIDGET_H
#define GMWIDGET_H

#include <QWidget>
#include <QQuickItem>
#include <QGeoCoordinate>
#include "SiteWidget.h"
#include "RuptureWidget.h"
#include "GMPEWidget.h"
#include "IntensityMeasureWidget.h"
#include "RecordSelectionWidget.h"
#include "ScenarioProcessor.h"
#include "SiteConfigWidget.h"
#include "LocationsListModel.h"
#include "GmAppConfig.h"
#include "SiteResultsWidget.h"

class GMWidget : public QWidget
{
    Q_OBJECT
    Q_ENUMS(ApplicationMode)
    Q_PROPERTY(ApplicationMode mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString cacheLocation READ getCacheLocation CONSTANT)

public:
    explicit GMWidget(QWidget *parent = 0);
    ~GMWidget();
    enum ApplicationMode{Edit = 0, Results = 1};
    ApplicationMode mode() const;

signals:
    void modeChanged(ApplicationMode newMode);
    void locationsChanged();

public slots:
    void setMode(ApplicationMode mode);
    void updateLocations();
    void setAppConfig();
    void exportAll();

private:
    RuptureWidget* m_ruptureWidget;
    PointSourceRupture* m_eqRupture;
    GMPE* m_gmpe;
    GMPEWidget* m_gmpeWidget;
    IntensityMeasure* m_intensityMeasure;
    IntensityMeasureWidget* m_intensityMeasureWidget;
    RecordSelectionConfig* m_selectionconfig;
    RecordSelectionWidget* m_selectionWidget;
    SiteResult* m_siteResult;
    ScenarioProcessor* m_scenarioProcessor;
    SiteConfig* m_siteConfig;
    SiteConfigWidget* m_siteConfigWidget;
    ApplicationMode m_mode;
    LocationsListModel* m_locationsModel;
    QQuickItem* m_mapObject;
    QStatusBar* m_statusBar;
    QLabel* m_mainStatus;
    QLabel* m_detailedStatus;
    QLabel* m_modeStatus;
    QProgressBar* m_progressBar;
    QPushButton* m_runButton;
    QPushButton* m_settingButton;
    GmAppConfig* m_appConfig;
    QLabel* m_alertIconWidget;
    QGeoCoordinate m_mapCenter;
    SiteResultsWidget* m_resultsWidget;

    //actions
    QAction* m_saveAction;
    QAction* m_loadAction;
    QAction* m_exportAction;
    QAction* m_exitAction;
    QAction* m_settingsAction;
    QAction* m_setLAAction;
    QAction* m_setSFAction;
    QAction* m_setSEAAction;
    QAction* m_setANCAction;
    QAction* m_aboutAction;
    QAction* m_licenseAction;
    QAction* m_defaultMapAction;
    QAction* m_terrainMapAction;

    void setupConnections();
    void initAppConfig();
    void initActions();
    void saveAppSettings();
    void setMapCenter(double latitude, double longitude);
    void exportFile(QString fileName, QString directory);
    QString getCacheLocation();
};

#endif // GMWIDGET_H
