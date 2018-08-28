#include "PGAResult.h"

PGAResult::PGAResult(QObject *parent) : QObject(parent)
{
    m_mean = 0.0;
}

void PGAResult::setMean(double mean)
{
    if(m_mean != mean)
    {
        m_mean = mean;
        emit meanChanged(m_mean);
    }
}

double PGAResult::intraEvStdDev() const
{
    return m_intraEvStdDev;
}

void PGAResult::setIntraEvStdDev(double intraEvStdDev)
{
    m_intraEvStdDev = intraEvStdDev;
}

double PGAResult::interEvStdDev() const
{
    return m_interEvStdDev;
}

void PGAResult::setInterEvStdDev(double interEvStdDev)
{
    m_interEvStdDev = interEvStdDev;
}

double PGAResult::stdDev() const
{
    return m_stdDev;
}

void PGAResult::setStdDev(double stdDev)
{
    m_stdDev = stdDev;
}

double PGAResult::mean() const
{
    return m_mean;
}
