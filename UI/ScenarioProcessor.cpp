#include <QJsonDocument>
#include <QProcess>
#include <QFile>
#include "ScenarioProcessor.h"

ScenarioProcessor::ScenarioProcessor(Site &site, PointSourceRupture &rupture, GMPE &gmpe,
                                     IntensityMeasure &intensityMeasure, RecordSelectionConfig &selectionConfig,
                                     QObject *parent) : QObject(parent), m_site(site), m_rupture(rupture),
                                     m_gmpe(gmpe), m_intensityMeasure(intensityMeasure), m_selectionConfig(selectionConfig)
{

}

void ScenarioProcessor::processScenario()
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

    QProcess scenarioProcess;
    //TODO: This will need to be configured
    QString EQScenarioPath = "C:\\SourceTree\\Simcenter-EQSS\\EQScenario\\build\\EQScenario.jar";

    QString inputName = "Scenario.json";
    QString OutputName = "Scenario_" + m_intensityMeasure.type()+ ".json";
    QStringList args;
    args << "-jar" << EQScenarioPath << inputName << OutputName;

    scenarioProcess.start("java", args);

    scenarioProcess.waitForFinished();
}
