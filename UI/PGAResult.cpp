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

double PGAResult::mean() const
{
    return m_mean;
}
