#include "GMRecord.h"

GMRecord::GMRecord(int id, double magnitude, double distance, double vs30, 
                std::string hzFile1, std::string hzFile2, std::string vlFile, std::vector<double> spectrum)
{
    this->m_Id = id;
    this->m_Magnitude = magnitude;
    this->m_Distance = distance;
    this->m_Vs30 = vs30;
    this->m_HzFile1 = hzFile1;
    this->m_HzFile2 = hzFile2;
    this->m_VlFile = vlFile;
    this->m_Source = "NGA West 2";
    this->m_Spectrum = spectrum;
}

std::string GMRecord::GetSource()
{
    return this->m_Source;
}

int GMRecord::GetId()
{
    return this->m_Id;
}

double GMRecord::GetMagnitude()
{
    return this->m_Magnitude;
}

std::vector<double> GMRecord::GetSpectrum()
{
    return this->m_Spectrum;
}

bool GMRecord::CheckCriteria(SelectionCriteria& criteria)
{
    Range distanceRange = criteria.Distance();
    if(this->m_Distance < distanceRange.Min() || this->m_Distance > distanceRange.Max())
        return false;

    Range magnitudeRange = criteria.Magnitude();
    if(this->m_Magnitude < magnitudeRange.Min() || this->m_Magnitude > magnitudeRange.Max())
        return false;

    Range vs30Range = criteria.Vs30();
    if(this->m_Vs30 < vs30Range.Min() || this->m_Vs30 > vs30Range.Max())
        return false;

    return true;
}

std::string GMRecord::GetHorizontalFile1()
{
	return this->m_HzFile1;
}

std::string GMRecord::GetHorizontalFile2()
{
	return this->m_HzFile2;
}

std::string GMRecord::GetVerticalFile()
{
	return this->m_VlFile;
}