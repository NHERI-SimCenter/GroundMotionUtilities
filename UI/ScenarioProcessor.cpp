#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include "ScenarioProcessor.h"

ScenarioProcessor::ScenarioProcessor(Site &site, PointSourceRupture &rupture, GMPE &gmpe,
                                     IntensityMeasure &intensityMeasure, RecordSelectionConfig &selectionConfig,
                                     QObject *parent) : QObject(parent), m_site(site), m_rupture(rupture),
                                     m_gmpe(gmpe), m_intensityMeasure(intensityMeasure), m_selectionConfig(selectionConfig)
{
    setupConnections();
}

void ScenarioProcessor::killProcesses()
{
    if(m_hazardAnalysisProcess.state() >= QProcess::ProcessState::Starting)
    {
        m_hazardAnalysisProcess.kill();
        m_hazardAnalysisProcess.waitForFinished();
    }

    if(m_simulationProcess.state() >= QProcess::ProcessState::Starting)
    {
        m_simulationProcess.kill();
        m_simulationProcess.waitForFinished();
    }

    if(m_recordSelectionProcess.state() >= QProcess::ProcessState::Starting)
    {
        m_recordSelectionProcess.kill();
        m_recordSelectionProcess.waitForFinished();
    }
}

void ScenarioProcessor::startProcessingScenario()
{
    emit statusUpdated("Started processing scenario");
    startHazardAnalysis();
}

void ScenarioProcessor::startHazardAnalysis()
{
    QJsonObject scenarioInput;
    scenarioInput.insert("Site", m_site.getJson());
    scenarioInput.insert("EqRupture", m_rupture.getJson());
    scenarioInput.insert("GMPE", m_gmpe.getJson());
    scenarioInput.insert("IntensityMeasure", m_intensityMeasure.getJson());

    QJsonDocument inputDoc(scenarioInput);
    QFile inputFile("Scenario.json");
    if (!inputFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't write scenario input file.");
        return;
    }
    inputFile.write(inputDoc.toJson());
    inputFile.close();

    //TODO: This will need to be configured
    QString EQScenarioPath = "C:\\SourceTree\\Simcenter-EQSS\\EQScenario\\build\\EQScenario.jar";

    QString inputName = "Scenario.json";
    QString OutputName = "Scenario_" + m_intensityMeasure.type()+ ".json";
    QStringList args;
    args << "-jar" << EQScenarioPath << inputName << OutputName;

    m_hazardAnalysisProcess.start("java", args);

    if(!m_hazardAnalysisProcess.waitForStarted(-1))
    {
        emit statusUpdated("Failed to start seismic hazard analysis!");
        return;
    }


}

void ScenarioProcessor::startSimulation()
{

}

void ScenarioProcessor::startSelection()
{

}

void ScenarioProcessor::processHazardOutput()
{
    QByteArray output = m_hazardAnalysisProcess.readAllStandardOutput();
    qDebug().nospace().noquote() << output;
}

void ScenarioProcessor::processSimulationOutput()
{
    QByteArray output = m_simulationProcess.readAllStandardOutput();
    qDebug().nospace().noquote() << output;
}

void ScenarioProcessor::processSelectionOutput()
{
    QByteArray output = m_recordSelectionProcess.readAllStandardOutput();
    qDebug().nospace().noquote() << output;
}

void ScenarioProcessor::startProcessingOutputs()
{

}

void ScenarioProcessor::setupConnections()
{
    //connecting hazard analysis output to process hazard output slot
    connect(&m_hazardAnalysisProcess, &QProcess::readyReadStandardOutput, this, &ScenarioProcessor::processHazardOutput);

    //connecting simulation output to process simulation output slot
    connect(&m_simulationProcess, &QProcess::readyReadStandardOutput, this, &ScenarioProcessor::processSimulationOutput);

    //connecting selection output to process selection output slot
    connect(&m_recordSelectionProcess, &QProcess::readyReadStandardOutput, this, &ScenarioProcessor::processSelectionOutput);

    //connecting hazard analysis finish to simulation start
    connect(&m_recordSelectionProcess, QOverload<int>::of(&QProcess::finished), [this](int){this->startSimulation();});

    //connecting simulation finish to selection start
    connect(&m_simulationProcess, QOverload<int>::of(&QProcess::finished), [this](int){this->startSelection();});

    //connecting selection finish to process the outputs
    connect(&m_recordSelectionProcess, QOverload<int>::of(&QProcess::finished), [this](int){this->startProcessingOutputs();});
}
