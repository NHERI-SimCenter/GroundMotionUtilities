#include <float.h>
#include "SelectionCriteria.h"


SelectionCriteria::SelectionCriteria()
{
    this->m_Magnitude.Set(0.0, DBL_MAX);
    this->m_Distance.Set(0.0, DBL_MAX);
    this->m_Vs30.Set(0.0, DBL_MAX);
}


void SelectionCriteria::SetMagnitudeRange(double min, double max)
{
    this->m_Magnitude.Set(min, max);
}

void SelectionCriteria::SetDistanceRange(double min, double max)
{
    this->m_Distance.Set(min, max);
}

void SelectionCriteria::SetVs30Range(double min, double max)
{
    this->m_Vs30.Set(min, max);
}

Range SelectionCriteria::Magnitude()
{
    return this->m_Magnitude;
}

Range SelectionCriteria::Distance()
{
    return this->m_Distance;
}

Range SelectionCriteria::Vs30()
{
    return this->m_Vs30;
}