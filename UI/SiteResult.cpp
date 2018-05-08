#include "SiteResult.h"

SiteResult::SiteResult(QObject *parent):QObject(parent)
{
    m_recordId = -1;
    m_scaleFactor = 0.0;
}

int SiteResult::recordId() const
{
    return m_recordId;
}

void SiteResult::setRecordId(int recordId)
{
    if(m_recordId != recordId)
    {
        m_recordId = recordId;
        emit recordChanged(m_recordId);
    }
}

double SiteResult::scaleFactor() const
{
    return m_scaleFactor;
}

void SiteResult::setScaleFactor(double scaleFactor)
{
    if(m_scaleFactor != scaleFactor)
    {
        m_scaleFactor = scaleFactor;
        emit scaleChanged(m_scaleFactor);
    }
}

void SiteResult::setMeans(QVector<double> means)
{
    m_means = means;
    emit meanChanged(getMean0());
}

Location &SiteResult::location()
{
    return m_location;
}

Location *SiteResult::getLocationPtr()
{
    return &m_location;
}

double SiteResult::getMean0()
{
    if(m_means.size() > 0)
        return m_means[0];
    else
        return 0;
}
