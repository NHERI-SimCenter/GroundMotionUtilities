#include <fstream>
#include <math.h>
#include "SelectionTarget.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

using namespace rapidjson;

SelectionTarget::SelectionTarget(SelectionConfig config)
{
    this->m_Type = config.GetTargetType();
    this->GetTargetSpectrums(config.TargetFile());
}

DiscretizedFunction SelectionTarget::GetSpectrum(int i)
{
    return this->m_Targets[i];
}

int SelectionTarget::NumTargets()
{
    return this->m_Targets.size();
}

void SelectionTarget::GetTargetSpectrums(std::string filename)
{
    Document doc;
    std::ifstream ifs(filename.c_str());
    IStreamWrapper isw(ifs);
    doc.ParseStream(isw);

    rapidjson::Value::Array periods = doc["Periods"].GetArray();

    Value::Array GMs = doc["GroundMotions"].GetArray();
    m_Locations.resize(GMs.Size());
    for (int i = 0; i < GMs.Size(); i++)
    {
        DiscretizedFunction targetSpectrum(periods.Size());
        Value targetValue;
        if(m_Type == TargetType::Mean)
        {
            targetValue = GMs[i]["SA"]["Mean"];
        }
        else if(m_Type == TargetType::Simulated)
        {
            targetValue = GMs[i]["SA"]["Simulations"].GetArray()[0];
        }
        Value::Array SAs = targetValue.GetArray();

        if(SAs.Size() != periods.Size())
            throw SHAException("Invalid spectrum size!");
        
        for (int j = 0; j < periods.Size(); j++)
        {
            targetSpectrum.AddPoint(periods[j].GetDouble(), exp(SAs[j].GetDouble()));
        }
        this->m_Targets.push_back(targetSpectrum);

        Value::Object Location = GMs[i]["Location"].GetObject();
            
        m_Locations[i].Set(Location["Longitude"].GetDouble(), Location["Latitude"].GetDouble());
    }
}

std::vector<Point> SelectionTarget::GetLocations()
{
    return this->m_Locations;
}