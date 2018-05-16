#ifndef SCENARIOPROCESSOR_H
#define SCENARIOPROCESSOR_H

#include <QObject>
#include <QProcess>
#include "SiteConfig.h"
#include "PointSourceRupture.h"
#include "GMPE.h"
#include "IntensityMeasure.h"
#include "RecordSelectionConfig.h"
#include "SiteResultsModel.h"
#include "GmAppConfig.h"

class ScenarioProcessor : public QObject
{
    Q_OBJECT

public:
    explicit ScenarioProcessor(GmAppConfig& appconfig, SiteConfig& siteConfig, PointSourceRupture& rupture, GMPE& gmpe, IntensityMeasure& intensityMeasure,
                               RecordSelectionConfig& selectionConfig, SiteResult& siteResult, QObject *parent = nullptr);
    SiteResultsModel& getResultsModel();

signals:
    void statusUpdated(QString status);
    void progressUpdated(QString detail);
    void progressUpdated(double percent);
    void stepFinished(QString stepName);
    void stepFailed(QString stepName);
    void finished();
    void failed();

public slots:
    void killProcesses();
    void startProcessingScenario();

private slots:
    void startHazardAnalysis();
    void startSimulation();
    void startSelection();
    void startProcessingOutputs();
    void processHazardOutput();
    void processSimulationOutput();
    void processSelectionOutput();

private:
    SiteConfig& m_siteConfig;
    PointSourceRupture& m_rupture;
    GMPE& m_gmpe;
    IntensityMeasure& m_intensityMeasure;
    RecordSelectionConfig& m_selectionConfig;
    SiteResult& m_siteResult;
    GmAppConfig& m_appConfig;

    QProcess m_hazardAnalysisProcess;
    QProcess m_simulationProcess;
    QProcess m_recordSelectionProcess;
    QString m_workDir;
    QString getWorkFilePath(QString filename);
    SiteResultsModel m_resultsModel;
    QString m_output;

    void setupConnections();
    void processOutputLines();
};

#endif // SCENARIOPROCESSOR_H
