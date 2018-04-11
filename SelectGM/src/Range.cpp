#include <float.h>
#include "Range.h"

Range::Range()
{
    Range(-DBL_MAX,DBL_MAX);
}

Range::Range(double min, double max)
{
    this->Set(min, max);
}

void Range::Set(double min, double max)
{
    if (min > max)
        throw "Invalid range!";

    this->m_Max = max;
    this->m_Min = min;
}

double Range::Max()
{
    return this->m_Max;
}

double Range::Min()
{
    return this->m_Min;
}
