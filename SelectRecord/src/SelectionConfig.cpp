#include <fstream>
#include <string>
#include "SelectionConfig.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

using namespace rapidjson;

SelectionConfig::SelectionConfig()
{
}

SelectionConfig::SelectionConfig(SelectionCriteria criteria, std::string databaseFile, std::string targetFile)
{
    this->m_Criteria = criteria;
    this->m_DatabaseFile = databaseFile;
    this->m_TargetFile = targetFile;
}

SelectionConfig::SelectionConfig(const SelectionConfig& config)
{
    this->m_Criteria = config.m_Criteria;
    this->m_DatabaseFile = config.m_DatabaseFile;
    this->m_TargetFile = config.m_TargetFile;
    this->m_DatabaseType = config.m_DatabaseType;
    this->m_TargetType = config.m_TargetType;
}

SelectionCriteria SelectionConfig::Criteria()
{
    return m_Criteria;
}

std::string SelectionConfig::DatabaseFile()
{
    return m_DatabaseFile;
}

std::string SelectionConfig::TargetFile()
{
    return m_TargetFile;
}

void SelectionConfig::ReadFromFile(const char* configFile)
{
    Document doc;
    std::ifstream ifs(configFile);
    IStreamWrapper isw(ifs);
    doc.ParseStream(isw);
    
    if(!doc.HasMember("Target"))
        throw "Target config is not specified!";
    if(!doc.HasMember("Database"))
        throw "Database config is not specified!";
    if(!doc.HasMember("Criteria"))
        throw "Selection criteria is not specified!";

    m_TargetFile = std::string(doc["Target"]["File"].GetString());
    m_DatabaseFile = std::string(doc["Database"]["File"].GetString());
    
    m_TargetType = ParseTargetType(doc["Target"]["Type"].GetString());
    m_DatabaseType = ParseDbType(doc["Database"]["Type"].GetString());

    if(doc["Criteria"].HasMember("Magnitude"))
    {
        double magnitudeMax = doc["Criteria"]["Magnitude"]["Max"].GetDouble();
        double magnitudeMin = doc["Criteria"]["Magnitude"]["Min"].GetDouble();
        m_Criteria.SetMagnitudeRange(magnitudeMin, magnitudeMax);
    }

    if(doc["Criteria"].HasMember("Distance"))
    {
        double distanceMax = doc["Criteria"]["Distance"]["Max"].GetDouble();
        double distanceMin = doc["Criteria"]["Distance"]["Min"].GetDouble();
        m_Criteria.SetDistanceRange(distanceMin, distanceMax);
    }

    if(doc["Criteria"].HasMember("Vs30"))
    {
        double vs30Max = doc["Criteria"]["Vs30"]["Max"].GetDouble();
        double vs30Min = doc["Criteria"]["Vs30"]["Min"].GetDouble();
        m_Criteria.SetVs30Range(vs30Min, vs30Max);
    }
}

DbType SelectionConfig::ParseDbType(const char* dbType)
{
    if(0 == strcmp(dbType, "CSV"))
    {
        return DbType::CSV;
    }
    else if(0 == strcmp(dbType, "SQLite"))
    {
        return DbType::SQLite;
    }
    else
        throw SHAException("Invalid records Db Type!");
}

TargetType SelectionConfig::ParseTargetType(const char* targetType)
{
    if(0 == strcmp(targetType, "MeanSpectrum"))
    {
        return TargetType::Mean;
    }
    else if(0 == strcmp(targetType, "SimulatedSpectrum"))
    {
        return TargetType::Simulated;
    }
    else
        throw SHAException("Invalid records Db Type!");
}

TargetType SelectionConfig::GetTargetType()
{
    return this->m_TargetType;
}
