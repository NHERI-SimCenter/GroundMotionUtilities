#include <cmath>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include "ScenarioProcessor.h"
#include <QStandardPaths>
#include <QDir>
#include <QRegularExpressionMatch>
#include "GmCommon.h"

ScenarioProcessor::ScenarioProcessor(GmAppConfig &appconfig, SiteConfig& siteConfig, PointSourceRupture &rupture, GMPE &gmpe,
                                     IntensityMeasure &intensityMeasure, RecordSelectionConfig &selectionConfig, SiteResult& siteResult,
                                     QObject *parent) : QObject(parent), m_siteConfig(siteConfig), m_rupture(rupture),
                                     m_gmpe(gmpe), m_intensityMeasure(intensityMeasure), m_selectionConfig(selectionConfig), m_siteResult(siteResult),
                                     m_appConfig(appconfig)
{
    setupConnections();
}

SiteResultsModel &ScenarioProcessor::getResultsModel()
{
    return m_resultsModel;
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
    emit statusUpdated("Processing seismic hazard analysis...");

    QJsonObject scenarioInput;
    scenarioInput.insert("Site", m_siteConfig.getJson());
    scenarioInput.insert("EqRupture", m_rupture.getJson());
    scenarioInput.insert("GMPE", m_gmpe.getJson());
    scenarioInput.insert("IntensityMeasure", m_intensityMeasure.getJson());

    QJsonDocument inputDoc(scenarioInput);
    QFile inputFile(GmCommon::getWorkFilePath("Scenario.json"));
    if (!inputFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't write scenario input file.");
        return;
    }
    inputFile.write(inputDoc.toJson());
    inputFile.close();

    QString inputName =  GmCommon::getWorkFilePath("Scenario.json");
    QString outputName = GmCommon::getWorkFilePath("Scenario_SHA.json");
    QStringList args;
    args << "-jar" << m_appConfig.eqHazardPath() << inputName << outputName;

    m_hazardAnalysisProcess.start("java", args);

    if(!m_hazardAnalysisProcess.waitForStarted(-1))
    {
        emit statusUpdated("Failed to start seismic hazard analysis!");
        return;
    }
}

void ScenarioProcessor::startSimulation()
{
    //First we need to check hazard analysis did not fail
    if(m_hazardAnalysisProcess.exitStatus() == QProcess::ExitStatus::CrashExit || m_hazardAnalysisProcess.exitCode() != 0)
    {
        emit statusUpdated("Hazard analysis failed!");
        return;
    }
    emit statusUpdated("Simulating intensity measure...");

    updatePeriods();
    QString hazardOutputPath = GmCommon::getWorkFilePath("Scenario_SHA.json");
    QString simConfigPath = GmCommon::getWorkFilePath("SimConfig.json");
    QString simOutputPath = GmCommon::getWorkFilePath("SimOutput.json");

    QJsonObject gmConfig;
    gmConfig.insert("File", hazardOutputPath);

    QJsonObject simulationConfig;
    simulationConfig.insert("GroundMotions", gmConfig);
    simulationConfig.insert("NumSimulations", 1);
    simulationConfig.insert("SpatialCorrelation", m_intensityMeasure.IsCorrelated());

    QJsonDocument simConfigDoc(simulationConfig);
    QFile simConfigFile(simConfigPath);
    if (!simConfigFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't write simulation config file.");
        return;
    }
    simConfigFile.write(simConfigDoc.toJson());
    simConfigFile.close();


    QStringList args;
    args << simConfigPath << simOutputPath;
    m_simulationProcess.start(m_appConfig.simulateIMPath(), args);

    if(!m_simulationProcess.waitForStarted(-1))
    {
        emit statusUpdated("Failed to start ground motion simulation!");
        return;
    }

}

void ScenarioProcessor::startSelection()
{
    emit statusUpdated("Selecting and scaling ground motion...");

    //First we need to check IM simulation
    if(m_simulationProcess.exitStatus() == QProcess::ExitStatus::CrashExit || m_simulationProcess.exitCode() != 0)
        return;

    QString selectionConfigPath = GmCommon::getWorkFilePath("SelectionConfig.json");
    QString simOutputPath = GmCommon::getWorkFilePath("SimOutput.json");
    QString selectionOutputPath = GmCommon::getWorkFilePath("SelectionOutput.json");


    QJsonObject selectionConfig = m_selectionConfig.getJson();
    QJsonObject target = selectionConfig["Target"].toObject();
    target.insert("File", simOutputPath);
    selectionConfig.insert("Target", target);

    QJsonObject db =  selectionConfig["Database"].toObject();
    if(m_selectionConfig.database() == RecordSelectionConfig::NGAWest2)
        db.insert("File", m_appConfig.NGAW2DbPath());
    else
        db.insert("File", m_appConfig.NGAW2SubsetDbPath());

    selectionConfig.insert("Database", db);

    QJsonDocument selectionConfigDoc(selectionConfig);
    QFile selectionConfigFile(selectionConfigPath);
    if (!selectionConfigFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't write record selection config file.");
        return;
    }
    selectionConfigFile.write(selectionConfigDoc.toJson());
    selectionConfigFile.close();

    QStringList args;
    args << selectionConfigPath << selectionOutputPath;
    m_recordSelectionProcess.start(m_appConfig.selectRecordPath(), args);

    if(!m_recordSelectionProcess.waitForStarted(-1))
    {
        emit statusUpdated("Failed to start ground motion record selection!");
        return;
    }
}

void ScenarioProcessor::processHazardOutput()
{
    QByteArray output = m_hazardAnalysisProcess.readAllStandardOutput();
    m_output = m_output.append(QString(output));
    processOutputLines();
}

void ScenarioProcessor::processSimulationOutput()
{
    QByteArray output = m_simulationProcess.readAllStandardOutput();
    m_output = m_output.append(QString(output));
    processOutputLines();
}

void ScenarioProcessor::processSelectionOutput()
{
    QByteArray output = m_recordSelectionProcess.readAllStandardOutput();
    m_output = m_output.append(QString(output));
    processOutputLines();
}

void ScenarioProcessor::startProcessingOutputs()
{
    emit statusUpdated("Processing outputs...");

    //We will open the record selection output
    QString selectionOutputPath = GmCommon::getWorkFilePath("SelectionOutput.json");
    QFile selectionOutputFile(selectionOutputPath);
    if(!selectionOutputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Couldn't read record selection outputs!");
        return;
    }

    QString output = selectionOutputFile.readAll();
    QJsonDocument selectionOutputDoc = QJsonDocument::fromJson(output.toUtf8());
    QJsonArray selectionResults = selectionOutputDoc.object()["GroundMotions"].toArray();

    //We will also open the hazard analysis output to read the site location
    QString hazardOutputPath = GmCommon::getWorkFilePath("SimOutput.json");
    QFile hazardOutputFile(hazardOutputPath);
    if(!hazardOutputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Couldn't read hazard analysis outputs!");
        return;
    }
    output = hazardOutputFile.readAll();
    QJsonDocument hazardOutputDoc = QJsonDocument::fromJson(output.toUtf8());
    QJsonArray hazardResults = hazardOutputDoc.object()["GroundMotions"].toArray();

    //reading periods
    //Reading SA Std Dev
    QJsonValue period;
    QVector<double> periods;
    foreach (period,  hazardOutputDoc.object()["Periods"].toArray())
        periods.append(period.toDouble());

    if(periods[0] < 0.01)
        periods[0] = 0.01;

    //Clearing results before adding new ones
    m_resultsModel.clear();
    for(int i = 0; i < selectionResults.size(); i++)
    {
        QSharedPointer<SiteResult> newResult = m_resultsModel.createNewResult();

        QJsonObject location = hazardResults[i].toObject()["Location"].toObject();
        newResult->location().set(location["Latitude"].toDouble(), location["Longitude"].toDouble());

        QJsonArray siteSelectionResults = selectionResults[i].toArray();
        newResult->recordSelection().clear();

        for (auto selectionResult: siteSelectionResults)
        {
            QSharedPointer<RecordSelection> selection(new RecordSelection());
            selection->setRecordId(selectionResult.toObject()["Record"].toObject()["Id"].toInt());
            selection->setScaleFactor(selectionResult.toObject()["ScaleFactor"].toDouble());
            newResult->recordSelection().push_back(selection);
        }

        //Reading PGA
        QJsonObject pgaResut = hazardResults[i].toObject()["PGA"].toObject();
        newResult->pgaResult().setMean(std::exp(pgaResut["Mean"].toDouble()));
        newResult->pgaResult().setStdDev(pgaResut["TotalStdDev"].toDouble());
        newResult->pgaResult().setInterEvStdDev(pgaResut["InterEvStdDev"].toDouble());
        newResult->pgaResult().setIntraEvStdDev(pgaResut["IntraEvStdDev"].toDouble());

        //Reading SA Mean
        QJsonObject saResult = hazardResults[i].toObject()["SA"].toObject();
        QJsonValue saMean;
        QVector<double> saMeans;
        foreach (saMean, saResult["Mean"].toArray())
            saMeans.append(std::exp(saMean.toDouble()));
        newResult->saResult().setMeans(saMeans);

        //Reading SA Std Dev
        QJsonValue saStdDev;
        QVector<double> saStdDevs;
        foreach (saStdDev, saResult["TotalStdDev"].toArray())
            saStdDevs.append(saStdDev.toDouble());
        newResult->saResult().setStdDevs(saStdDevs);

        //Reading SA Inter Event Std Dev
        QVector<double> saInterEvStdDevs;
        foreach (saStdDev, saResult["InterEvStdDev"].toArray())
            saInterEvStdDevs.append(saStdDev.toDouble());
        newResult->saResult().setInterEvStdDevs(saInterEvStdDevs);

        //Reading SA Intra Event Std Dev
        QVector<double> saIntraEvStdDevs;
        foreach (saStdDev, saResult["IntraEvStdDev"].toArray())
            saIntraEvStdDevs.append(saStdDev.toDouble());
        newResult->saResult().setIntraEvStdDevs(saIntraEvStdDevs);

        //Reading Simulated Spectrum
        QJsonValue simulatedSA;
        QVector<double> simulateSpectrum;
        foreach (simulatedSA, saResult["Simulations"].toArray()[0].toArray())
            simulateSpectrum.append(std::exp(simulatedSA.toDouble()));
        newResult->setSimulatedSpectrum(simulateSpectrum);

        //setting periods
        newResult->setPeriods(periods);

        //Reading Site Data
        QJsonArray siteDataResuts = hazardResults[i].toObject()["SiteData"].toArray();
        QJsonValue siteDataJson;

        foreach (siteDataJson, siteDataResuts)
        {
            QJsonObject siteDataObject = siteDataJson.toObject();
            newResult->addSiteData(siteDataObject["Type"].toString(), siteDataObject["Value"].toVariant());
            newResult->addSiteDataSource(siteDataObject["Type"].toString(), siteDataObject["Source"].toVariant());
        }
    }

    //TODO, single site result
    QJsonObject result = selectionResults[0].toObject();
    QSharedPointer<RecordSelection> selection(new RecordSelection());
    selection->setRecordId(result["Record"].toObject()["Id"].toInt());
    selection->setScaleFactor(result["ScaleFactor"].toDouble());
    m_siteResult.recordSelection().push_back(selection);

    emit progressUpdated("Finished record selection and scaling");
    emit statusUpdated("Analysis completed successfully!");
    emit finished();
    emit progressUpdated("");
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
    connect(&m_hazardAnalysisProcess, QOverload<int>::of(&QProcess::finished), [this](int){this->startSimulation();});

    //connecting simulation finish to selection start
    connect(&m_simulationProcess, QOverload<int>::of(&QProcess::finished), [this](int){this->startSelection();});

    //connecting selection finish to process the outputs
    connect(&m_recordSelectionProcess, QOverload<int>::of(&QProcess::finished), [this](int){this->startProcessingOutputs();});
}

void ScenarioProcessor::processOutputLines()
{
    QString line;
    QStringList lines =  m_output.split(QRegExp("\n|\r\n|\r"), QString::SkipEmptyParts);
    m_output.clear();

    if(lines.count() > 100)
        for (int i = 0; i < lines.count() - 100; i++ )
            lines.removeAt(i);

    QRegularExpression siteExp("Processing Site ([0-9]+)");
    QRegularExpression simulationExp("\\t Processing period: ([\\d\\.]+)");
    QRegularExpression selectionExp("Selecting record for target ([0-9]+)");


    foreach (line, lines)
    {
        emit progressUpdated(line.trimmed());

        QRegularExpressionMatch siteMatch = siteExp.match(line);
        if (siteMatch.hasMatch())
        {
            int siteNo = siteMatch.captured(1).toInt();
            emit progressUpdated((double)siteNo/m_siteConfig.siteGrid().getNumSites());
        }

        QRegularExpressionMatch simulationMatch = simulationExp.match(line);
        if (simulationMatch.hasMatch())
        {
            double period = simulationMatch.captured(1).toDouble();
            int count = m_periods.indexOf(period) + 1;
            emit progressUpdated((double)count/m_periods.size());
        }

        QRegularExpressionMatch selectionMatch = selectionExp.match(line);
        if (selectionMatch.hasMatch())
        {
            int targetNo = selectionMatch.captured(1).toInt();
            emit progressUpdated((double)targetNo/m_siteConfig.siteGrid().getNumSites());
        }
    }
}

void ScenarioProcessor::updatePeriods()
{
    QString hazardOutputPath = GmCommon::getWorkFilePath("Scenario_SHA.json");
    QFile hazardOutputFile(hazardOutputPath);
    if(!hazardOutputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Couldn't read hazard analysis outputs!");
        return;
    }
    QString output = hazardOutputFile.readAll();
    QJsonDocument hazardOutputDoc = QJsonDocument::fromJson(output.toUtf8());
    QJsonArray periods = hazardOutputDoc.object()["Periods"].toArray();
    QJsonValue period;
    m_periods.clear();
    foreach(period, periods)
    {
        m_periods.append(period.toDouble());
    }
    if(m_periods[0] < 0.001)
        m_periods[0] = 0.001;

    hazardOutputFile.close();
}
