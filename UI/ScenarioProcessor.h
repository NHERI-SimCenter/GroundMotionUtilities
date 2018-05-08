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

class ScenarioProcessor : public QObject
{
    Q_OBJECT

public:
    explicit ScenarioProcessor(SiteConfig& siteConfig, PointSourceRupture& rupture, GMPE& gmpe, IntensityMeasure& intensityMeasure,
                               RecordSelectionConfig& selectionConfig, SiteResult& siteResult, QObject *parent = nullptr);
    SiteResultsModel& getResultsModel();

signals:
    void statusUpdated(QString status);
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

    QProcess m_hazardAnalysisProcess;
    QProcess m_simulationProcess;
    QProcess m_recordSelectionProcess;
    QString m_workDir;
    QString getWorkFilePath(QString filename);
    SiteResultsModel m_resultsModel;

    void setupConnections();
};

#endif // SCENARIOPROCESSOR_H
