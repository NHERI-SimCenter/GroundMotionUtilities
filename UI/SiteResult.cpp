#include "SiteResult.h"

SiteResult::SiteResult(QObject *parent):QObject(parent)
{

}

Location &SiteResult::location()
{
    return m_location;
}

SAResult &SiteResult::saResult()
{
    return m_saResult;
}

PGAResult &SiteResult::pgaResult()
{
    return m_pgaResult;
}

QVector<QSharedPointer<RecordSelection>> &SiteResult::recordSelection()
{
    return m_recordSelection;
}

QVector<double> SiteResult::periods()
{
    return m_periods;
}

QVector<double> SiteResult::simulatedSpectrum()
{
    return m_simulatedSpectrum;
}

void SiteResult::addSiteData(QString name, QVariant value)
{
    m_siteData[name] = value;
}

void SiteResult::addSiteDataSource(QString name, QVariant value)
{
    m_siteDataSources[name] = value;
}

void SiteResult::clearSiteData()
{
    m_siteData.clear();
}

void SiteResult::clearSiteDataSource()
{
    m_siteDataSources.clear();
}

QHash<QString, QVariant> &SiteResult::siteData()
{
    return m_siteData;
}

QHash<QString, QVariant> &SiteResult::siteDataSources()
{
    return m_siteDataSources;
}

void SiteResult::setPeriods(QVector<double> periods)
{
    m_periods = periods;
}

void SiteResult::setSimulatedSpectrum(QVector<double> simulatedSpectrum)
{
    m_simulatedSpectrum = simulatedSpectrum;
}

Location *SiteResult::getLocationPtr()
{
    return &m_location;
}

SAResult *SiteResult::getSAResultPtr()
{
    return &m_saResult;
}

PGAResult *SiteResult::getPGAResultPtr()
{
    return &m_pgaResult;
}

RecordSelection *SiteResult::getRecordSelectionPtr()
{
    return m_recordSelection[0].data();
}
